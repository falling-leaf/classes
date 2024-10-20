import utils.ConnectConfig;
import utils.DatabaseConnector;

import com.sun.net.httpserver.*;

import java.net.InetSocketAddress;

import java.io.IOException;
import java.util.logging.Logger;

public class Main {
    private static final Logger log = Logger.getLogger(Main.class.getName());

    public static void main(String[] args) throws IOException{
        try {
            // parse connection config from "resources/application.yaml"
            ConnectConfig conf = new ConnectConfig();
            log.info("Success to parse connect config. " + conf.toString());
            // connect to database
            DatabaseConnector connector = new DatabaseConnector(conf);
            boolean connStatus = connector.connect();
            if (!connStatus) {
                log.severe("Failed to connect database.");
                System.exit(1);
            }
            /* do somethings */
            HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);
            server.createContext("/card", new CardHandler(connector));
            server.createContext("/borrow", new BorrowHandler(connector));
            server.createContext("/book", new BookHandler(connector));
            server.start();

            System.out.println("Server is listening on port 8000");
            // release database connection handler
            if (connector.release()) {
                log.info("Success to release connection.");
            } else {
                log.warning("Failed to release connection.");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}