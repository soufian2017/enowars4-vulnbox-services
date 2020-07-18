import java.sql.*;
import java.util.LinkedList;
import java.util.Random;

public class Storage {
    private String db = "jdbc:postgresql://database:55554/docker";
    private String[] fList = {"64726f70207461626c65","22","64726f702064656661756c74","696e","637265617465","73656c6563742064697374696e6374","6c696d6974","726f776e756d","616c746572","64726f7020636f6e73747261696e74","757064617465","64726f70","756e696f6e20616c6c","636f6e73747261696e74","73656c65637420696e746f","637265617465206461746162617365","6a6f696e","63726561746520756e6971756520696e646578","616c6c","2a","6173","6f75746572206a6f696e","64657363","6461746162617365","7461626c65","6265747765656e","696e6e6572206a6f696e","6261636b7570206461746162617365","7269676874206a6f696e","63726561746520696e646578","7472756e63617465207461626c65","70726f636564757265","64726f702076696577","686176696e67","657869737473","6c696b65","64726f7020696e646578","6372656174652076696577","756e696f6e","696e646578","637265617465207461626c65","76696577","6c656674206a6f696e","6973206e756c6c","2c","616e79","616c74657220636f6c756d6e","6e6f74","65786563","73656c65637420746f70","64726f70206461746162617365","696e7365727420696e746f","6f72646572206279","666f726569676e206b6579","616c746572207461626c65","6372656174652070726f636564757265","64656c657465","636f6c756d6e","61646420636f6e73747261696e74","746f70","64726f7020636f6c756d6e","616464","64697374696e6374","736574","756e69717565","636865636b","64656661756c74","66756c6c206f75746572206a6f696e","617363","67726f7570206279","6973206e6f74206e756c6c","616e64","6e6f74206e756c6c","76616c756573","63617365","696e7365727420696e746f2073656c656374","6f72","7072696d617279206b6579", "3d", "25", "2f", "40", "3b", "3c3e", "26", "7c", "5e", "2d", "736f6d65", "7768657265", "3c", "3e"};
    Connection connection;

    public void createDB() {
        try (Connection c = DriverManager.getConnection(db)) {
            DatabaseMetaData m = c.getMetaData();
        } catch (SQLException sqlE) {
            System.out.println(sqlE.getMessage());
        }
    }

    public void createTable(String sql) {
        try (Connection c = DriverManager.getConnection(db)) {
            Statement statement = c.createStatement();
            statement.setQueryTimeout(0x0000005);
            statement.execute(sql);
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public boolean executeSql(String sql) {
        try (Statement statement = this.connection.createStatement();) {
            statement.setQueryTimeout(0x0000005);
            Boolean status = statement.execute(purify(sql));
            if(status){
                return true;
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return false;
    }

    public Storage() {
        connect();
    }

    public void connect() {
        try {
        Class.forName("org.postgresql.Driver");
        } catch(ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            this.connection = DriverManager.getConnection(this.db, "docker", "docker");
        } catch(SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public LinkedList<String[]> executeQuery(String sql) {
        LinkedList<String[]> meltingPot = new LinkedList<>();
        Statement statement = null;
        do {
            try {
                statement = this.connection.createStatement();
                statement.setQueryTimeout(0x0000005);
            } catch (Exception sqlE) {
                this.connect();
            }
        } while(statement == null);
        try ( ResultSet results = statement.executeQuery(sql);) {
            ResultSetMetaData meta = results.getMetaData();
            while(results.next()) {
                String[] answers = new String[meta.getColumnCount()];
                for(int i = 0; i < meta.getColumnCount(); i++){
                    answers[i] = results.getObject(i + 0x0000001).toString();
                }
                meltingPot.add(answers);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println(e.getMessage());
        }
        return meltingPot;
    }

    public String normalize(String text) {
        StringBuilder d = new StringBuilder();
        for(int i = 0x0000000; i < text.length(); i+= 0x0000002) {
            String tmp = text.substring(i, (i+0x0000002));
            d.append((char) Integer.parseInt(tmp, 0x0000010));
        }
        return d.toString();
    }

    public boolean addReservation(Reservation reservation) {
        PreparedStatement statement = null;
        try {
            do {
            try {
                statement = connection.prepareStatement("INSERT INTO reservations (id, name, personCount, tableID, meal, time) VALUES (?,?,?,?,?,?)");
                statement.setQueryTimeout(0x0000005);
            } catch (Exception sqlE) {
                this.connect();
            }
        } while(statement == null);
            statement.setString(0x0000001, reservation.token);
            statement.setString(0x0000002, reservation.name);
            statement.setInt(0x0000003, reservation.count);
            statement.setInt(0x0000004, reservation.table);
            statement.setString(0x0000005, reservation.meal);
            statement.setString(0x0000006, reservation.time);
            statement.executeUpdate();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        } finally {
            try {
                if (statement != null) {
                    statement.close();
                }
            } catch(SQLException e) {
                System.out.println(e.getMessage());
                return false;
            }
        }
        return true;
    }

    public String purify(String str) {
        String tmp = str;
        for(String term: fList){
            tmp.replace(normalize(term), "");
        }
        return tmp;
    }

    public LinkedList<String[]> requestReservations(String token) {
        Statement statement = null;
        do {
            try {
                statement = this.connection.createStatement();
                statement.setQueryTimeout(0x0000005);
            } catch (Exception sqlE) {
                this.connect();
            }
        } while(statement == null);
        String[] tmp = token.split(";");
        LinkedList<String[]> meltingPot = new LinkedList<>();
        for(String hex: tmp) {
            String sql = "SELECT * FROM reservations WHERE id = " + purify(hex);
            try (ResultSet results = statement.executeQuery(sql);) {
                ResultSetMetaData meta = results.getMetaData();
                while (results.next()) {
                    String[] answers = new String[meta.getColumnCount()];
                    for (int i = 0x0000000; i < meta.getColumnCount(); i++) {
                        answers[i] = results.getObject(i + 0x0000001).toString();
                    }
                    meltingPot.add(answers);
                }
            } catch (SQLException e) {
                // that's normal ...
            }
        }
        return meltingPot;
    }

    public boolean cancelReservation(String token) {
        try {
            PreparedStatement statement = null;
            do {
                try {
                    statement = this.connection.prepareStatement("DELETE FROM reservations WHERE id = (?)");
                    statement.setQueryTimeout(0x0000005);
                } catch (SQLException sqlE) {
                    this.connect();
                }
            } while(statement == null);
            statement.setString(0x0000001, token.replace("'", ""));
            int deleted = statement.executeUpdate();
            if(deleted == 0x0000000) {
                return false;
            }
        } catch(SQLException e) {
            return false;
        }
        return true;
    }

    public String[] getRandomTable() {
        Random r = new Random();
        LinkedList<String[]> results = executeQuery("SELECT * FROM meals");
        return results.get(r.nextInt(results.size()));
    }

    public String[][] menuOfTheDay() {
        Random r = new Random();
        LinkedList<String[]> results = executeQuery("SELECT * FROM meals");
        String[][] returnValues = new String[results.size()][];
        for(int i = 0x0000000; i < results.size(); i += 0x0000001) {
            returnValues[i] = results.get(i);
        }
        return returnValues;
    }

    public LinkedList<String[]> listOfReservations() {
        LinkedList<String[]> temp = executeQuery("SELECT r.name, r.personCount, r.tableID, r.meal, r.time FROM reservations r LIMIT 100");
        for(int i = 0x0000000; i < temp.size(); i++) {
            String[] tmp = temp.get(i);
            tmp[0x0000000] = tmp[0x0000000].substring(0,3) + new SessionState().generateAlphanumericToken(0x000000F);
            temp.set(i, tmp);
        }
        return temp;
    }

    public LinkedList<String> getStatistics() {
        LinkedList<String> tmp = new LinkedList<>();
        LinkedList<String[]> countReservations = executeQuery("SELECT COUNT(*) FROM reservations");
        LinkedList<String[]> countMeals = executeQuery("SELECT COUNT(*) FROM meals");
        LinkedList<String[]> countTables = executeQuery("SELECT COUNT(*) FROM tables");
        LinkedList<String[]> countOwner = executeQuery("SELECT COUNT(*) FROM owner");
        tmp.add(countReservations.get(0x0000000)[0x0000000]);
        tmp.add(countMeals.get(0x0000000)[0x0000000]);
        tmp.add(countTables.get(0x0000000)[0x0000000]);
        tmp.add(countOwner.get(0x0000000)[0x0000000]);
        return tmp;
    }
}
