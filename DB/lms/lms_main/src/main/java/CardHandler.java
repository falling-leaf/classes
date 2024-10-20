import java.io.IOException;
import java.io.OutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import com.sun.net.httpserver.*;

import entities.Card;
import entities.Card.CardType;
import queries.*;
import utils.DatabaseConnector;
import java.net.URI;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.StandardCharsets;

public class CardHandler implements HttpHandler{

    private DatabaseConnector connector;
    private LibraryManagementSystem library;

    CardHandler(DatabaseConnector connector){
        this.connector = connector;
    }
    
    @Override
    public void handle(HttpExchange exchange) throws IOException{
        Headers headers = exchange.getResponseHeaders();
        connector.connect();
        library = new LibraryManagementSystemImpl(connector);
        headers.add("Access-Control-Allow-Origin", "*");
        headers.add("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
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
            ApiResult result = library.showCards();
            CardList resCardList = (CardList) result.payload;
            String response = "[";
            for (int i = 0; i < resCardList.getCount(); i++){
                Card target = resCardList.getCards().get(i);
                if (i != 0)
                    response += ",";
                response += "{\"id\":"+ target.getCardId() + 
                ",\"name\": \"" + target.getName() + 
                "\", \"department\": \"" + target.getDepartment() + 
                "\", \"type\": \"" + target.getType().toString() + "\"}";
                
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

    private void handlePostRequest(HttpExchange exchange) throws IOException{
        try {
            InputStream requestBody = exchange.getRequestBody();
            BufferedReader reader = new BufferedReader(new InputStreamReader(requestBody, StandardCharsets.UTF_8));
            StringBuilder requestBodyBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                requestBodyBuilder.append(line);
            }

            Boolean statusset = true;
            // 读出来的数据类似：{"name":"why","department":"ZJU","type":"学生"}
            // 考虑利用正则表达式对它进行处理，这里要注意大括号需要被正则转义，所以读出来要为"\{"才行
            String template = "\\{\"name\":\"(.+?)\",\"department\":\"(.+?)\",\"type\":\"(.+?)\"\\}";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(requestBodyBuilder.toString());
            if (matcher.find()) {
                String inname = matcher.group(1);
                String indept = matcher.group(2);
                String intype = matcher.group(3);
            
                //创建新的借书证
                Card newCard = new Card();
                newCard.setDepartment(indept);
                newCard.setName(inname);
                if (intype.equals("学生"))//这里有大坑！必须用equals才行！
                    newCard.setType(CardType.values("S"));
                else newCard.setType(CardType.values("T"));
                ApiResult rscheck = library.registerCard(newCard);
                if (!rscheck.ok)
                    statusset = false;
            } else {
                System.out.println("No match found");
                throw new Exception();
            }
            
            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (statusset)
                outputStream.write("Card created successfully".getBytes());
            else outputStream.write("Card created not successfully".getBytes());
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, -1);
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
            String template = "\\{\"id\":(.+?),\"name\":\"(.+?)\",\"department\":\"(.+?)\",\"type\":\"(.+?)\"\\}";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(requestBodyBuilder.toString());
            if (matcher.find()) {
                int inid = Integer.parseInt(matcher.group(1));
                String inname = matcher.group(2);
                String indept = matcher.group(3);
                String intype = matcher.group(4);
                //创建新的借书证
                Card newCard = new Card();
                newCard.setCardId(inid);
                newCard.setDepartment(indept);
                newCard.setName(inname);
                if (intype.equals("学生"))//这里有大坑！必须用equals才行！
                    newCard.setType(CardType.values("S"));
                else newCard.setType(CardType.values("T"));
                library.modifyCardInfo(newCard);

            } else {
                System.out.println("No match found");
                throw new Exception();
            }
            
            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            outputStream.write("Card modified successfully".getBytes());
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
            Boolean statusset = true;
            String template = "toRemove=(.+)";
            Pattern pattern = Pattern.compile(template);
            Matcher matcher = pattern.matcher(query);
            if (matcher.find()) {
                String rmID = matcher.group(1);
                int targetID = Integer.parseInt(rmID);
                ApiResult rscheck = library.removeCard(targetID);
                if (!rscheck.ok)
                statusset = false;
            } else {
                System.out.println("No match found");
                throw new Exception();
            }

            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, 0);
            OutputStream outputStream = exchange.getResponseBody();
            if (statusset)
                outputStream.write("Card deleted successfully".getBytes());
            else outputStream.write("Card deleted not successfully".getBytes());
            outputStream.close();
        } catch (Exception e){
            exchange.sendResponseHeaders(405, 0);
        }
    }
}

