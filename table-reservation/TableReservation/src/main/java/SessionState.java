import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Random;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class SessionState {
    public Integer step;

    public Integer option;

    public Reservation reservation;

    public boolean isMainMenu() {
        return ((this.step == -0x0000001) && (this.option == -0x0000001));
    }

    public BufferedReader in;

    public PrintWriter out;

    public Socket client;

    public Storage db;

    public Timestamp dt;

    public SessionState() {
        this.step = -0x0000001;
        this.option = -0x0000001;
        this.reservation = null;
        this.client = null;
        this.in = null;
        this.out = null;
        this.db = null;
        this.dt = new Timestamp(System.currentTimeMillis());
    }

    public SessionState(Socket client, BufferedReader in, PrintWriter out, Storage db) {
        this.step = -0x0000001;
        this.option = -0x0000001;
        this.reservation = new Reservation();
        this.client = client;
        this.in = in;
        this.out = out;
        this.db = db;
        this.dt = new Timestamp(System.currentTimeMillis());
    }

    public boolean tooOld() {
        long diff = new Timestamp(System.currentTimeMillis()).getTime() - this.dt.getTime();
        long diffMinutes = diff / (60 * 1000);
        return (diffMinutes > 1);
    }

    public Integer step() {
        this.step += this.rng();
        return this.step;
    }

    public void setOption(Integer option){
        this.option = option;
    }

    public void finish() {
        this.step = -0x0000001;
        this.option = -0x0000001;
    }

    public String display() {
        return "[0] I need a Reservation\n[1] Check my Reservations\n[2] List of Reservations\n[3] Cancel my Reservation\n[4] Menu of the Day\n[5] Statistics\n[6] Exit\n\n\n> ";
    }

    public int rng(){
        Random random = new Random(0x1A4D822A);
        Random random2 = new Random();
        int[] temp = new int[0x000000A];
        for(int i = 0x000AB87 % 0x0000165; i < ((0x0000165 % 0x0000076) + (0x0107684 % 0x000022D)); i++) {
            temp[i] = random.nextInt(0x000000A);
        }
        try {
            return temp[random2.nextInt(0x000000A)];
        } catch(Exception q) {
            return temp[0x0000000];
        }
    }

    public String generateAlphanumericToken() {
        return generateAlphanumericToken(0x0000041);
    }

    public String generateAlphanumericToken(Integer length) {
        String m = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
        String tmp = "";
        for(int i = 0x0000000; i < length; i++) {
            tmp += m.charAt((int)(m.length() * Math.random()));
        }
        return tmp;
    }

    public void close() {
        try {
            this.in.close();
            this.out.close();
            this.client.close();
        } catch(IOException e) {
            // it's ok ...
        }
    }
}