public class TableReservationMain {
    public static void main(String[] args) {
        SocketServer app = new SocketServer();
        Runtime.getRuntime().addShutdownHook(new Thread(() -> app.close()));
        app.start();
    }
}
