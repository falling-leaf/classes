import java.io.IOException;
import java.io.OutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import com.sun.net.httpserver.*;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import entities.Borrow;

import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import queries.*;
import utils.DatabaseConnector;
import java.net.URI;
import java.nio.charset.StandardCharsets;

public class BorrowHandler implements HttpHandler{

    private DatabaseConnector connector;
    private LibraryManagementSystem library;

    BorrowHandler(DatabaseConnector connector){
        this.connector = connector;
    }
    
    @Override
    public void handle(HttpExchange exchange) throws IOException{
        Headers headers = exchange.getResponseHeaders();
        connector.connect();
        library = new LibraryManagementSystemImpl(connector);
        headers.add("Access-Control-Allow-Origin", "*");
        headers.add("Access-Control-Allow-Methods", "GET, PUT");
        headers.add("Access-Control-Allow-Headers", "Content-Type");

        String requestMethod = exchange.getRequestMethod();

        if (requestMethod.equals("GET")){
            handleGetRequest(exchange);
        } else if (requestMethod.equals("OPTIONS")){
            handleOptionsRequest(exchange);
        } else if (requestMethod.equals("PUT")){
            handlePutRequest(exchange);
        } else {
            exchange.sendResponseHeaders(405, -1);
        }
    }

    private void handleGetRequest(HttpExchange exchange) throws IOException{
        try {
            URI requestURI = exchange.getRequestURI();  
            String query = requestURI.getRawQuery();
            //此时字符串大概长成这样："cardID=123"
            String template = "cardID=(.+)";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(query);
            String response = "[";
            //tabledata的项目有：cardID, bookID, borrowtime, returntime
            if (matcher.find()) {
                String cardID = matcher.group(1);
                int targetID = Integer.parseInt(cardID);
                ApiResult rs = library.showBorrowHistory(targetID);
                BorrowHistories histories = (BorrowHistories) rs.payload;
                for (int i = 0; i < histories.getCount(); i++){
                    BorrowHistories.Item target = histories.getItems().get(i);
                    String realtime = Long.toString(target.getBorrowTime());
                    String borrowtime = realtime.substring(0, 4) + "." + 
                                        realtime.substring(4, 6) + "." +
                                        realtime.substring(6, 8) + " " +
                                        realtime.substring(8, 10) + ":" +
                                        realtime.substring(10, 12) + ":" +
                                        realtime.substring(12, 14);
                    String returntime;
                    if (target.getReturnTime() != 0){
                        String realreturn = Long.toString(target.getReturnTime());
                        returntime = realreturn.substring(0, 4) + "." + 
                                        realreturn.substring(4, 6) + "." +
                                        realreturn.substring(6, 8) + " " +
                                        realreturn.substring(8, 10) + ":" +
                                        realreturn.substring(10, 12) + ":" +
                                        realreturn.substring(12, 14);
                    } else returntime = "未归还";
                    if (i != 0)
                        response += ",";
                    response += "{\"cardID\":"+ target.getCardId() + 
                    ",\"bookID\": " + target.getBookId() + 
                    ", \"borrowTime\": \"" + borrowtime + 
                    "\", \"returnTime\": \"" + returntime + "\"}";
                }
            } else {
                System.out.println("No match found");
                throw new Exception();
            }
            response += "]";

            OutputStream outputStream = exchange.getResponseBody();
            exchange.getResponseHeaders().set("Content-Type", "application/json; charset=utf-8");
            exchange.sendResponseHeaders(200, 0);
            outputStream.write(response.getBytes(StandardCharsets.UTF_8));
            outputStream.close();

        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
        }
    }

    private void handlePutRequest(HttpExchange exchange) throws IOException{
        try {
            InputStream requestBody = exchange.getRequestBody();
            BufferedReader reader = new BufferedReader(new InputStreamReader(requestBody));
            StringBuilder requestBodyBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                requestBodyBuilder.append(line);
            }

            Boolean statuscons = true;
            String template = "\\{\"bookID\":(.+?),\"cardID\":(.+?)\\}";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(requestBodyBuilder.toString());
            if (matcher.find()){
                int bookID = Integer.parseInt(matcher.group(1));
                int cardID = Integer.parseInt(matcher.group(2));
                Borrow borrow = new Borrow();
                borrow.setBookId(bookID);
                borrow.setCardId(cardID);
                LocalDateTime now = LocalDateTime.now();
                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyMMddHHmmss");
                String formattedNow = now.format(formatter);
                long return_time = Long.parseLong(formattedNow);
                borrow.setReturnTime(return_time);
                ApiResult rscheck = library.returnBook(borrow);
                if (!rscheck.ok)
                    statuscons = false;
            }
            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (!statuscons)
                outputStream.write("book returned failed".getBytes());
            else outputStream.write("book returned successfully".getBytes());
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
        }
        
    }

    private void handleOptionsRequest(HttpExchange exchange) throws IOException{
        exchange.getResponseHeaders().set("Content-Type", "text/plain");
        exchange.sendResponseHeaders(204, -1);
    }
}

