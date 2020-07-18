import java.io.*;
import java.net.*;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SocketServer {
    private AtomicBoolean doStop = new AtomicBoolean();
    private HashMap<String, TableReservationCommand> commands;
    private int port = 0x000D903;
    private ServerSocket serverSocket;
    private ConcurrentHashMap<String, SessionState> activeClients = new ConcurrentHashMap<String, SessionState>();
    private LinkedList<String> clientsToRemove = new LinkedList<String>();
    private Storage db;
    private ExecutorService executor = Executors.newFixedThreadPool(0x0000028);;


    public SocketServer() {
        TableReservationCommand[] commandList = { new TableReservationCommand.Create(), new TableReservationCommand.List(), new TableReservationCommand.Cancel(), new TableReservationCommand.Menu(), new TableReservationCommand.Check(), new TableReservationCommand.Statistics() };
        commands = new HashMap<String, TableReservationCommand>(commandList.length);
        for (TableReservationCommand e : commandList) {
            commands.put(e.name(), e);
        }
    }

    public void start() {
        try {
            this.serverSocket = new ServerSocket(port);
        } catch(IOException e) {
            e.printStackTrace();
        }
        this.doStop.set(false);
        this.db = new Storage();
        new Thread(() -> {
            this.acceptConnections();
        }).start();
        new Thread(() -> {this.run();}).start();
    }
    public void processMessage(String id) {
        SessionState state = activeClients.get(id);
        String request = null;
        try {
            request = state.in.readLine();
        } catch(IOException e) {
            e.printStackTrace();
        }
        request = request.replace("\n", "").replace("\r", "");
        System.out.println("Msg: ` " + request + " `");
        TableReservationCommand cmd = null;
        if (request.equals(Integer.toString(0x0000000)) || state.option == 0x0000000) {
            state.setOption(0x0000000);
            cmd = commands.get("CREATE");
        } else if (request.equals(Integer.toString(0x0000001)) || state.option == 0x0000001) {
            state.setOption(0x0000001);
            cmd = commands.get("CHECK");
        } else if (request.equals(Integer.toString(0x0000002)) || state.option == 0x0000002) {
            state.setOption(0x0000002);
            cmd = commands.get("LIST");
        } else if (request.equals(Integer.toString(0x0000003)) || state.option == 0x0000003) {
            state.setOption(0x0000003);
            cmd = commands.get("CANCEL");
        } else if (request.equals(Integer.toString(0x0000004)) || state.option == 0x0000004) {
            state.setOption(0x0000004);
            cmd = commands.get("MENU");
        } else if(request.equals(Integer.toString(0x0000005)) || state.option == 0x0000005) {
            state.setOption(0x0000005);
            cmd = commands.get("STATS");
        } else if (request.equals(Integer.toString(0x0000006)) || state.option == 0x0000006) {
            state.close();
            this.clientsToRemove.add(id);
        }

        if(cmd != null) {
            cmd.call(request, state);
        } else {
            state.out.print(state.display());
            state.out.flush();
        }
    }

    public void run() {
        while(true) {
            if(activeClients.isEmpty()) {
                try {
                    Thread.sleep(0x00000C8);
                } catch(Exception e) {
                    e.printStackTrace();
                }
                continue;
            }
            activeClients.forEach((String id, SessionState state) -> {
                try {
                    if(state.in.ready()) {
                        executor.execute(() -> { processMessage(id); });
                    }
                } catch(IOException e) {

                }
                if(state.tooOld()) {
                    state.out.println("Closing connection...");
                    this.clientsToRemove.add(id);
                }
            });

            this.clientsToRemove.forEach((String id) -> {
                this.activeClients.get(id).close();
                this.activeClients.remove(id);
            });
            this.clientsToRemove.clear();
            try {
                Thread.sleep(0x0000064);
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    private void acceptConnections(){
        while(true) {
            try {
                Socket clientSocket = serverSocket.accept();
                String identifier = generateIdentifier();
                clientSocket.setKeepAlive(true);
                clientSocket.setTcpNoDelay(true);
                BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                SessionState state = new SessionState(clientSocket, in, out, this.db);
                state.out.print(state.display());
                state.out.flush();
                this.activeClients.put(identifier, state);
            } catch (Exception e) {}
        }
    }
    private String generateIdentifier() {
        String uniqueID;
        do {
            uniqueID = UUID.randomUUID().toString();
        } while(this.activeClients.get(uniqueID) != null);
        return uniqueID;
    }

    public void close() {
        if(this.activeClients != null) {
            this.activeClients.forEach((id, state) -> {
                state.close();
            });
        }
    }
}