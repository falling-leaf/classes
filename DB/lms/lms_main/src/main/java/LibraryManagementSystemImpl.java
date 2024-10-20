//所有语句都要逐一进行验证！
//for storebook: need to check "if" condition
//对批量处理storebook: 需要设置中断语句
//注意：JDBC中SQL语句不用加分号！
//测试1需要用到querybook函数，此处暂且先注释掉，以便只评测函数1，之后要改回来
//通过测试明确：只要全绿那么函数就算过了
//函数2测试日志：182行左右，经过读取代码发现它需要我们在插入图书之后，改变java中对书的ID的值！
//函数2中同样需要同步book的stock！
//md，函数2中183行，对于invalid序号，要求是“抛出异常”而非“什么都不变”！这意味着需要提前特判
//由于批处理文件没有比较好的获取id的方法，因此不得不使用查找来把东西找回来
//函数3有问题，暂时先不做处理
//函数4由于实例以borrow/return为基础，暂时不做测试
//记录：price用double类型映射
//函数1、2、5、6过
//为防止注入攻击，所有statement改用preparedstatement重写（不能提前写作业啊）
//函数3，4可进一步简化
//结束力！！！并行处理加一个写锁就可以：for update；这个的作用就是在写操作与读操作并行时，要先等写完之后才能开锁
import entities.Book;
import entities.Borrow;
import entities.Card;
import entities.Card.CardType;
import queries.*;
import utils.DBInitializer;
import utils.DatabaseConnector;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class LibraryManagementSystemImpl implements LibraryManagementSystem {

    private final DatabaseConnector connector;

    public LibraryManagementSystemImpl(DatabaseConnector connector) {
        this.connector = connector;
    }

    @Override
    public ApiResult storeBook(Book book) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlinsertion = "insert into book values(0, ?, ?, ?, ?, ?, ?, ?)";
            PreparedStatement insertion = conn.prepareStatement(sqlinsertion, Statement.RETURN_GENERATED_KEYS);
            // //对于auto increment列，需要用0补位
            insertion.setString(1, book.getCategory());
            insertion.setString(2, book.getTitle());
            insertion.setString(3, book.getPress());
            insertion.setInt(4, book.getPublishYear());
            insertion.setString(5, book.getAuthor());
            insertion.setDouble(6, book.getPrice());
            insertion.setInt(7, book.getStock());
                
            insertion.executeUpdate();//当书目重复时，会因为数据库自带的unique而报错
            ResultSet generatedkey = insertion.getGeneratedKeys();
            if (generatedkey.next())
                book.setBookId(generatedkey.getInt(1));
            else throw new Exception();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult incBookStock(int bookId, int deltaStock) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlupdatestock = "update book set stock = stock + ? where book_id = ? and stock + ? >= 0";
            PreparedStatement updatestock = conn.prepareStatement(sqlupdatestock);
            updatestock.setInt(1, deltaStock);
            updatestock.setInt(2, bookId);
            updatestock.setInt(3, deltaStock);
            int affected = updatestock.executeUpdate();//更新好正常的stock
            if (affected == 0)
                throw new Exception();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult storeBook(List<Book> books) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlinsert = "insert into book values(0, ?, ?, ?, ?, ?, ?, ?)";
            PreparedStatement insertion = conn.prepareStatement(sqlinsert, Statement.RETURN_GENERATED_KEYS);
            for (int i = 0; i < books.size(); i++){
                    insertion.setString(1, books.get(i).getCategory());
                    insertion.setString(2, books.get(i).getTitle());
                    insertion.setString(3, books.get(i).getPress());
                    insertion.setInt(4, books.get(i).getPublishYear());
                    insertion.setString(5, books.get(i).getAuthor());
                    insertion.setDouble(6, books.get(i).getPrice());
                    insertion.setInt(7, books.get(i).getStock());
                    insertion.addBatch();
            }
            insertion.executeBatch();//批处理语句
            ResultSet generatedKeys = insertion.getGeneratedKeys();
            int counter = 0;
            while (generatedKeys.next())
                books.get(counter++).setBookId(generatedKeys.getInt(1));
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult removeBook(int bookId) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlremovebook = "delete from book where book_id = ? and not exists(select * from borrow where book_id = ? and return_time = 0)";
            PreparedStatement removebook = conn.prepareStatement(sqlremovebook);
            removebook.setInt(1, bookId);
            removebook.setInt(2, bookId);
            int affected = removebook.executeUpdate();
            if (affected == 0)
                throw new Exception();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult modifyBookInfo(Book book) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            //根据函数意图，认定它为根据书号，修改书的属性
            String sqlupdateinfo = "update book set category = ?, title = ?, press = ?, publish_year = ?, author = ?, price = ? where book_id = ?";
            PreparedStatement updateinfo = conn.prepareStatement(sqlupdateinfo);
            updateinfo.setString(1, book.getCategory());
            updateinfo.setString(2, book.getTitle());
            updateinfo.setString(3, book.getPress());
            updateinfo.setInt(4, book.getPublishYear());
            updateinfo.setString(5, book.getAuthor());
            updateinfo.setDouble(6, book.getPrice());
            updateinfo.setInt(7, book.getBookId());
            int affected = updateinfo.executeUpdate();
            if (affected == 0)
                throw new Exception();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult queryBook(BookQueryConditions conditions) {
        Connection conn = connector.getConn();//记录connector的conn
        List<Book> bookresult = new ArrayList<>();
        try {
            //经过读取代码确认，如果项目为null，则表示没有限定该条件
            String sqlquery = "select * from book where 1 = 1";//这是为了避免全为null的情况
            if (conditions.getCategory() != null)
                sqlquery += " and category = ?";
            if (conditions.getTitle() != null)
                sqlquery += " and title like ?";
            if (conditions.getPress() != null)
                sqlquery += " and press like ?";
            if (conditions.getAuthor() != null)
                sqlquery += " and author like ?";
            if (conditions.getMinPrice() != null)
                sqlquery += " and price >= ?";
            if (conditions.getMaxPrice() != null)
                sqlquery += " and price <= ?";
            if (conditions.getMinPublishYear() != null)
                sqlquery += " and publish_year >= ?";
            if (conditions.getMaxPublishYear() != null)
                sqlquery += " and publish_year <= ?";
            sqlquery += " order by " + conditions.getSortBy() + " " + conditions.getSortOrder() + " , book_id ASC";
            PreparedStatement querybook = conn.prepareStatement(sqlquery);
            int paramcount = 1;
            if (conditions.getCategory() != null)
                querybook.setString(paramcount++, conditions.getCategory());
            if (conditions.getTitle() != null)
                querybook.setString(paramcount++, "%" + conditions.getTitle() + "%");
            if (conditions.getPress() != null)
                querybook.setString(paramcount++, "%" + conditions.getPress() + "%");
            if (conditions.getAuthor() != null)
                querybook.setString(paramcount++, "%" + conditions.getAuthor() + "%");
            if (conditions.getMinPrice() != null)
                querybook.setDouble(paramcount++, conditions.getMinPrice());
            if (conditions.getMaxPrice() != null)
                querybook.setDouble(paramcount++, conditions.getMaxPrice());
            if (conditions.getMinPublishYear() != null)
                querybook.setInt(paramcount++, conditions.getMinPublishYear());
            if (conditions.getMaxPublishYear() != null)
                querybook.setInt(paramcount++, conditions.getMaxPublishYear());

            ResultSet rs = querybook.executeQuery();
            while (rs.next()){
                Book book = new Book();
                book.setBookId(rs.getInt("book_id"));
                book.setCategory(rs.getString("category"));
                book.setAuthor(rs.getString("author"));
                book.setPress(rs.getString("press"));
                book.setPublishYear(rs.getInt("publish_year"));
                book.setTitle(rs.getString("title"));
                book.setPrice(rs.getDouble("price"));
                book.setStock(rs.getInt("stock"));
                bookresult.add(book);
            }
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        BookQueryResults output = new BookQueryResults(bookresult);
        return new ApiResult(true, null, output);
    }

    @Override
    public ApiResult borrowBook(Borrow borrow) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlcheckreturn = "select * from borrow where card_id = ? and book_id = ? and return_time = 0";
            PreparedStatement checkreturn = conn.prepareStatement(sqlcheckreturn);
            checkreturn.setInt(1, borrow.getCardId());
            checkreturn.setInt(2, borrow.getBookId());
            ResultSet rscheckreturn = checkreturn.executeQuery();
            if (rscheckreturn.next())
                    throw new Exception();//这本书他借了没还

            String sqlcheckstock = "select * from book where book_id = ? and stock = 0 for update";
            PreparedStatement checkstock = conn.prepareStatement(sqlcheckstock);
            checkstock.setInt(1, borrow.getBookId());
            ResultSet rscheckstock = checkstock.executeQuery();
            if (rscheckstock.next())
                throw new Exception();//这本书没库存了

            String sqlupborrow = "insert into borrow values(?, ?, ?, ?)";
            PreparedStatement upborrow = conn.prepareStatement(sqlupborrow);
            upborrow.setInt(1, borrow.getCardId());
            upborrow.setInt(2, borrow.getBookId());
            upborrow.setLong(3, borrow.getBorrowTime());
            upborrow.setLong(4, 0);
            upborrow.executeUpdate();

            String sqlupstock = "update book set stock = stock - 1 where book_id = ?";
            PreparedStatement upstock = conn.prepareStatement(sqlupstock);
            upstock.setInt(1, borrow.getBookId());
            upstock.executeUpdate();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
        //return new ApiResult(false, "Unimplemented Function");
    }

    @Override
    public ApiResult returnBook(Borrow borrow) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlupreturn = "update borrow set return_time = ? where card_id = ? and book_id = ? and borrow_time < ? and return_time = 0";
            PreparedStatement upreturn = conn.prepareStatement(sqlupreturn);
            upreturn.setLong(1, borrow.getReturnTime());
            upreturn.setInt(2, borrow.getCardId());
            upreturn.setInt(3, borrow.getBookId());
            upreturn.setLong(4, borrow.getReturnTime());
            int affected = upreturn.executeUpdate();
            if (affected == 0)//没有这个记录或更新失败
                throw new Exception();

            String sqlupstock = "update book set stock = stock + 1 where book_id = ?";
            PreparedStatement upstock = conn.prepareStatement(sqlupstock);
            upstock.setInt(1, borrow.getBookId());
            upstock.executeUpdate();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult showBorrowHistory(int cardId) {
        Connection conn = connector.getConn();//记录connector的conn
        List<BorrowHistories.Item> resultItems = new ArrayList<>();
        try {
            System.out.println(cardId);
            String sqlseekborrow = "select * from borrow natural join book where card_id = ? order by borrow_time DESC, book_id ASC";
            PreparedStatement seekborrow = conn.prepareStatement(sqlseekborrow);
            seekborrow.setInt(1, cardId);
            ResultSet rsseekborrow = seekborrow.executeQuery();
            
            while (rsseekborrow.next()){
                Book book = new Book();
                book.setBookId(rsseekborrow.getInt("book_id"));
                book.setCategory(rsseekborrow.getString("category"));
                book.setAuthor(rsseekborrow.getString("author"));
                book.setPress(rsseekborrow.getString("press"));
                book.setPublishYear(rsseekborrow.getInt("publish_year"));
                book.setTitle(rsseekborrow.getString("title"));
                book.setPrice(rsseekborrow.getDouble("price"));
                book.setStock(rsseekborrow.getInt("stock"));
                Borrow borrow = new Borrow();
                borrow.setBookId(rsseekborrow.getInt("book_id"));
                borrow.setCardId(rsseekborrow.getInt("card_id"));
                borrow.setBorrowTime(rsseekborrow.getLong("borrow_time"));
                borrow.setReturnTime(rsseekborrow.getLong("return_time"));
                BorrowHistories.Item item = new BorrowHistories.Item(borrow.getCardId(), book, borrow);
                resultItems.add(item);
            }
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        BorrowHistories output = new BorrowHistories(resultItems);
        return new ApiResult(true, null, output);
    }

    @Override
    public ApiResult registerCard(Card card) {
        Connection conn = connector.getConn();//记录connector的conn
        try {//已经有了约束，因此不需要考虑卡片重复的情况
            String sqlupcard = "insert into card values(0, ?, ?, ?)";
            PreparedStatement upcard = conn.prepareStatement(sqlupcard, Statement.RETURN_GENERATED_KEYS);
            upcard.setString(1, card.getName());
            upcard.setString(2, card.getDepartment());
            upcard.setString(3, card.getType().getStr());
            upcard.executeUpdate();
            ResultSet generatedkey = upcard.getGeneratedKeys();
            if (generatedkey.next())
                card.setCardId(generatedkey.getInt(1));
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult removeCard(int cardId) {
        Connection conn = connector.getConn();//记录connector的conn
        try {
            String sqlremovecard = "delete from card where card.card_id = ? and not exists(select * from borrow where borrow.card_id = ? and return_time = 0)";
            PreparedStatement removecard = conn.prepareStatement(sqlremovecard);
            removecard.setInt(1, cardId);
            removecard.setInt(2, cardId);
            int affected = removecard.executeUpdate();
            if (affected == 0)
                throw new Exception();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    @Override
    public ApiResult showCards() {
        Connection conn = connector.getConn();//记录connector的conn
        List<Card> resultCards = new ArrayList<>();
        try {
            String sqlselectcard = "select * from card order by card_id ASC";
            PreparedStatement selectcard = conn.prepareStatement(sqlselectcard);
            ResultSet rsselectcard = selectcard.executeQuery();
            while (rsselectcard.next()){
                Card card = new Card();
                card.setCardId(rsselectcard.getInt("card_id"));
                card.setDepartment(rsselectcard.getString("department"));
                card.setName(rsselectcard.getString("name"));
                card.setType(CardType.values(rsselectcard.getString("type")));
                resultCards.add(card);
            }
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        CardList output = new CardList(resultCards);
        return new ApiResult(true, null, output);
    }

    @Override
    public ApiResult modifyCardInfo(Card card){
        Connection conn = connector.getConn();// 记录connector的conn
        try {
            String sqlupdatecard = "update card set department = ?, name = ?, type = ? where card_id = ?";
            PreparedStatement updatecard = conn.prepareStatement(sqlupdatecard);
            updatecard.setString(1, card.getDepartment());
            updatecard.setString(2, card.getName());
            updatecard.setString(3, card.getType().getStr());
            updatecard.setInt(4, card.getCardId());
            updatecard.executeUpdate();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }


    @Override
    public ApiResult resetDatabase() {
        Connection conn = connector.getConn();// 记录connector的conn
        try {
            Statement stmt = conn.createStatement();// 设置一个statement用于执行语句
            DBInitializer initializer = connector.getConf().getType().getDbInitializer();
            //目测这句话是用来对数据库类型进行设定的
            stmt.addBatch(initializer.sqlDropBorrow());
            stmt.addBatch(initializer.sqlDropBook());
            stmt.addBatch(initializer.sqlDropCard());
            stmt.addBatch(initializer.sqlCreateCard());
            stmt.addBatch(initializer.sqlCreateBook());
            stmt.addBatch(initializer.sqlCreateBorrow());
            stmt.executeBatch();//批处理语句
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    private void rollback(Connection conn) {
        try {
            conn.rollback();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void commit(Connection conn) {
        try {
            conn.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}
