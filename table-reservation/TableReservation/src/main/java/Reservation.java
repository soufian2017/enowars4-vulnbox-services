public class Reservation {

    String name;
    String token;
    Integer count;
    String meal;
    Integer table;
    String time;

    public Reservation() {
        this.name = "";
        this.token = "";
        this.count = 0;
        this.meal = "";
        this.table = 0;
        this.time = "";
    }

    public String toString() {
        return "[ " + this.name + " " + this.token + " " + this.count + " " + this.meal + " " + this.table + " " + this.time + " ]";
    }
}
