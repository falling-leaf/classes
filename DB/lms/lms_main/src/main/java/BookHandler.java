import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStreamReader;

import com.sun.net.httpserver.*;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import entities.Book;
import entities.Borrow;
import queries.*;
import utils.DatabaseConnector;
import java.net.URI;
import java.nio.charset.StandardCharsets;

public class BookHandler implements HttpHandler{

    private DatabaseConnector connector;
    private LibraryManagementSystem library;

    BookHandler(DatabaseConnector connector){
        this.connector = connector;
    }
    
    @Override
    public void handle(HttpExchange exchange) throws IOException{
        Headers headers = exchange.getResponseHeaders();
        connector.connect();
        library = new LibraryManagementSystemImpl(connector);
        headers.add("Access-Control-Allow-Origin", "*");
        headers.add("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS, PUT");
        headers.add("Access-Control-Allow-Headers", "Content-Type");

        String requestMethod = exchange.getRequestMethod();

        if (requestMethod.equals("GET")){
            handleGetRequest(exchange);
        } else if (requestMethod.equals("OPTIONS")){
            handleOptionsRequest(exchange);
        } else if (requestMethod.equals("POST")){
            handlePostRequest(exchange);
        } else if (requestMethod.equals("DELETE")){
            handleDeleteRequest(exchange);
        } else if (requestMethod.equals("PUT")){
            handlePutRequest(exchange);
        } else {
            exchange.sendResponseHeaders(405, -1);
        }
    }

    private void handleGetRequest(HttpExchange exchange) throws IOException{
        try {
            String querystring = exchange.getRequestURI().getQuery();
            ApiResult queryResult;
            Boolean numcons = true;
            if (querystring == null)
                queryResult = library.queryBook(new BookQueryConditions());
            else {
                String template = "toSearch=\\{\"category\":\"(.*?)\",\"title\":\"(.*?)\",\"press\":\"(.*?)\",\"author\":\"(.*?)\",\"publishYearmin\":\"(.*?)\",\"publishYearmax\":\"(.*?)\",\"pricemin\":\"(.*?)\",\"pricemax\":\"(.*?)\"\\}";
                Pattern pattern = Pattern.compile(template);
                Matcher matcher = pattern.matcher(querystring);
                BookQueryConditions bookquery = new BookQueryConditions();
                if (matcher.find()){
                    if (!matcher.group(1).equals(""))
                        bookquery.setCategory(matcher.group(1));
                    if (!matcher.group(2).equals(""))
                        bookquery.setTitle(matcher.group(2));
                    if (!matcher.group(3).equals(""))
                        bookquery.setPress(matcher.group(3));
                    if (!matcher.group(4).equals(""))
                        bookquery.setAuthor(matcher.group(4));
                    try {
                        if (!matcher.group(5).equals(""))
                            bookquery.setMinPublishYear(Integer.parseInt(matcher.group(5)));
                        if (!matcher.group(6).equals(""))
                            bookquery.setMaxPublishYear(Integer.parseInt(matcher.group(6)));
                        if (!matcher.group(7).equals(""))
                            bookquery.setMinPrice(Double.parseDouble(matcher.group(7)));
                        if (!matcher.group(8).equals(""))
                            bookquery.setMaxPrice(Double.parseDouble(matcher.group(8)));  
                    } catch (Exception e){
                        numcons = false;
                    }
                }
                queryResult = library.queryBook(bookquery);
            }
            BookQueryResults rsbooks = (BookQueryResults) queryResult.payload;
            String response = "[";
            for (int i = 0; i < rsbooks.getCount(); i++){
                Book target = rsbooks.getResults().get(i);
                if (i != 0)
                    response += ",";
                response += "{\"id\":"+ target.getBookId() + 
                ",\"category\": \"" + target.getCategory() +
                "\", \"title\": \"" + target.getTitle() + 
                "\", \"press\": \"" + target.getPress() + 
                "\", \"publishYear\":" + target.getPublishYear() + 
                ", \"author\": \"" + target.getAuthor() + 
                "\", \"price\":" + target.getPrice() + 
                ", \"stock\":" + target.getStock() + 
                "}";
            }
            response += "]";

            OutputStream outputStream = exchange.getResponseBody();
            exchange.getResponseHeaders().set("Content-Type", "application/json; charset=utf-8");
            exchange.sendResponseHeaders(200, 0);
            if (!numcons)
                outputStream.write("number constrain".getBytes());
            else outputStream.write(response.getBytes(StandardCharsets.UTF_8));
            outputStream.close();

        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
        }
    }

    private void handlePostRequest(HttpExchange exchange) throws IOException{
        try {
            InputStream requestBody = exchange.getRequestBody();
            BufferedReader reader = new BufferedReader(new InputStreamReader(requestBody, StandardCharsets.UTF_8));
            StringBuilder requestBodyBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                requestBodyBuilder.append(line);
            }

            Boolean numcons = true;
            Boolean statuscons = true;
            Boolean brcons = true;
            Boolean pathcons = true;
            Boolean storecons = true;
            Boolean newbookopt = false;
            Boolean newborrowopt = false;
            Boolean storebookopt = false;
            // 考虑利用正则表达式对它进行处理，这里要注意大括号需要被正则转义，所以读出来要为"\{"才行
            String template = "\\{\"category\":\"(.+?)\",\"title\":\"(.+?)\",\"press\":\"(.+?)\",\"publishYear\":\"(.+?)\",\"author\":\"(.+?)\",\"price\":\"(.+?)\",\"stock\":\"(.+?)\"\\}";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(requestBodyBuilder.toString());
            if (matcher.find()) {
                newbookopt = true;
                String incategory = matcher.group(1);
                String intitle = matcher.group(2);
                String inpress = matcher.group(3);
                String inauthor = matcher.group(5);
                int inpublishYear = 0, instock = 0;
                double inprice = 0.0;
                try {
                    inpublishYear = Integer.parseInt(matcher.group(4));
                    inprice = Double.parseDouble(matcher.group(6));
                    instock = Integer.parseInt(matcher.group(7));
                } catch (Exception e){
                    numcons = false;
                }
            
                if (numcons){
                    //创建新的借书证
                    Book newBook = new Book();
                    newBook.setCategory(incategory);
                    newBook.setTitle(intitle);
                    newBook.setPress(inpress);
                    newBook.setPublishYear(inpublishYear);
                    newBook.setAuthor(inauthor);
                    newBook.setPrice(inprice);
                    newBook.setStock(instock);
                    ApiResult rscheck = library.storeBook(newBook);
                    if (!rscheck.ok)
                        statuscons = false;
                }

            } else {
                String templatebr = "\\{\"bookID\":(.+?),\"cardID\":\"(.+?)\"\\}";
                Pattern patternbr = Pattern.compile(templatebr);
                Matcher matcherbr = patternbr.matcher(requestBodyBuilder.toString());
                if (matcherbr.find()){
                    newborrowopt = true;
                    int bookID = Integer.parseInt(matcherbr.group(1));
                    int cardID = Integer.parseInt(matcherbr.group(2));
                    Borrow borrow = new Borrow();
                    borrow.setBookId(bookID);
                    borrow.setCardId(cardID);
                    LocalDateTime now = LocalDateTime.now();
                    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyMMddHHmmss");
                    String formattedNow = now.format(formatter);
                    long borrow_time = Long.parseLong(formattedNow);
                    borrow.setBorrowTime(borrow_time);
                    ApiResult rscheckbr = library.borrowBook(borrow);
                    if (!rscheckbr.ok)
                        brcons = false;
                } else {
                    storebookopt = true;
                    String templatestore = "\\{\"path\":\"(.+?)\"\\}";
                    Pattern patternstore = Pattern.compile(templatestore);
                    Matcher matcherstore = patternstore.matcher(requestBodyBuilder.toString());
                    if (matcherstore.find()){
                        String path = matcherstore.group(1);
                        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
                            String inline;
                            List<Book> bookset = new ArrayList<Book>();
                            while ((inline = br.readLine()) != null) {//格式目前为：双引号内为内容，用逗号隔开
                                String templateinput = "\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\"";
                                Pattern patterninput = Pattern.compile(templateinput);
                                Matcher matcherinput = patterninput.matcher(inline);
                                if (matcherinput.find()){
                                    Book book = new Book();
                                    book.setCategory(matcherinput.group(1));
                                    book.setTitle(matcherinput.group(2));
                                    book.setPress(matcherinput.group(3));
                                    book.setAuthor(matcherinput.group(5));
                                    try {
                                        book.setPublishYear(Integer.parseInt(matcherinput.group(4)));
                                        book.setPrice(Double.parseDouble(matcherinput.group(6)));
                                        book.setStock(Integer.parseInt(matcherinput.group(7)));
                                        bookset.add(book);
                                    } catch (Exception e){
                                        System.out.println("the book" + book.getBookId() + "has number constraint and will be ignored.");
                                        continue;
                                    }
                                }
                            }
                            ApiResult rscheck = library.storeBook(bookset);
                            if (!rscheck.ok)
                                storecons = false;
                        } catch (IOException e){
                            pathcons = false;
                        } catch (Exception e){
                            storecons = false;
                        }
                    } else {
                        System.out.println("No match found");
                        throw new Exception();
                    }
                }
            }
            
            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (newbookopt){
                if (!numcons)
                    outputStream.write("number constrain".getBytes());
                else if (!statuscons)
                    outputStream.write("book existed".getBytes());
                else outputStream.write("book stored successfully".getBytes());
            } else if (newborrowopt){
                if (!brcons)
                    outputStream.write("borrow failed".getBytes());
                else outputStream.write("borrow successfully".getBytes());
            } else if (storebookopt){
                if (!pathcons)
                    outputStream.write("invalid path".getBytes());
                else if (!storecons)
                    outputStream.write("store failed".getBytes());
                else outputStream.write("store successfully".getBytes());
            }
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
        }
    }

    private void handleOptionsRequest(HttpExchange exchange) throws IOException{
        exchange.getResponseHeaders().set("Content-Type", "text/plain");
        exchange.sendResponseHeaders(204, -1);
    }

    private void handleDeleteRequest(HttpExchange exchange) throws IOException{
        try {
            URI requestURI = exchange.getRequestURI();  
            String query = requestURI.getRawQuery();
            //此时字符串大概长成这样："toRemove=123"
            String template = "toRemove=(.+)";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(query);
            Boolean statuscons = true;
            if (matcher.find()) {
                String rmID = matcher.group(1);
                int targetID = Integer.parseInt(rmID);
                ApiResult rscheck = library.removeBook(targetID);
                if (!rscheck.ok)
                    statuscons = false;
            } else {
                System.out.println("No match found");
                throw new Exception();
            }

            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (statuscons)
                outputStream.write("Book removed successfully".getBytes());
            else outputStream.write("Book removed failure".getBytes());
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, 0);
        }
    }

    private void handlePutRequest(HttpExchange exchange) throws IOException{
        try {
            InputStream requestBody = exchange.getRequestBody();
            BufferedReader reader = new BufferedReader(new InputStreamReader(requestBody, StandardCharsets.UTF_8));
            StringBuilder requestBodyBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                requestBodyBuilder.append(line);
            }

            Boolean numcons = true;
            Boolean statuscons = true;
            Boolean modifyinfo = false;
            Boolean stockinfo = false;
            String template = "\\{\"id\":(.+?),\"category\":\"(.+?)\",\"title\":\"(.+?)\",\"press\":\"(.+?)\",\"publishYear\":(.+),\"author\":\"(.+?)\",\"price\":(.+)\\}";
            System.out.println("Received POST request to create card with data: " + requestBodyBuilder.toString());
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(requestBodyBuilder.toString());
            if (matcher.find()) {
                modifyinfo = true;
                String incategory = matcher.group(2);
                String intitle = matcher.group(3);
                String inpress = matcher.group(4);
                String inauthor = matcher.group(6);
                int inpublishYear = 0, inid = 0;
                double inprice = 0.0;
                try {
                    inid = Integer.parseInt(matcher.group(1));
                    String inpubString = matcher.group(5);
                    if (inpubString.charAt(0) == '\"')
                        inpubString = inpubString.substring(1, inpubString.length() - 1);
                    inpublishYear = Integer.parseInt(inpubString);
                    String inpri = matcher.group(7);
                    if (inpri.charAt(0) == '\"')
                        inpri = inpri.substring(1, inpri.length() - 1);
                    inprice = Double.parseDouble(inpri);
                } catch (Exception e){
                    numcons = false;
                }
            
                if (numcons){
                    Book newBook = new Book();
                    newBook.setCategory(incategory);
                    newBook.setTitle(intitle);
                    newBook.setPress(inpress);
                    newBook.setPublishYear(inpublishYear);
                    newBook.setAuthor(inauthor);
                    newBook.setPrice(inprice);
                    newBook.setBookId(inid);
                    ApiResult rscheck = library.modifyBookInfo(newBook);
                    if (!rscheck.ok)
                        statuscons = false;
                }

            } else {
                String templatestock = "\\{\"bookID\":(.+?),\"stock\":\"(.+?)\"\\}";
                Pattern patternstock = Pattern.compile(templatestock);
                Matcher matcherstock = patternstock.matcher(requestBodyBuilder.toString());
                if (matcherstock.find()){
                    stockinfo = true;
                    try {
                        int bookID = Integer.parseInt(matcherstock.group(1));
                        int stockdel = Integer.parseInt(matcherstock.group(2));
                        ApiResult rscheck = library.incBookStock(bookID, stockdel);
                        if (!rscheck.ok)
                            statuscons = false;
                    } catch (Exception e){
                        numcons = false;
                    }
                } else {
                    System.out.println("No match found");
                    throw new Exception();
                }
            }
            
            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (modifyinfo){
                if (!numcons)
                    outputStream.write("number constrain".getBytes());
                else if (!statuscons)
                    outputStream.write("modifying failed".getBytes());//理论上不会出现
                else outputStream.write("book modified successfully".getBytes());
            } else if (stockinfo){
                if (!numcons)
                    outputStream.write("number constrain".getBytes());
                else if (!statuscons)
                    outputStream.write("changing stock failed".getBytes());
                else outputStream.write("stock changed successfully".getBytes());
            }
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
        }
    }
}

