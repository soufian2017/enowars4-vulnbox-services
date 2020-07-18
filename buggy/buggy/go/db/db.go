package db

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"time"

	_ "github.com/go-sql-driver/mysql"
)

type User struct {
	ID       int
	Username string
	Password string
	Status   string
	Bonus    int
	Admin    bool
}

type Order struct {
	ID       int
	Username string
	ItemID   int
	Color    string
	Quantity int
	Hash     string
}

type Message struct {
	To      string
	From    string
	Hash    string
	Content string
}

type Comment struct {
	Timestamp string
	User      string
	Product   string
	Content   string
}

type Ticket struct {
	User    string
	Subject string
	Hash    string
}

var db *sql.DB

func InitDB() {
	var err error
	db, err = sql.Open("mysql", fmt.Sprintf("root:%s@tcp(mysql:3306)/%s", os.Getenv("MYSQL_ROOT_PASSWORD"), os.Getenv("MYSQL_DATABASE")))
	db.SetMaxOpenConns(500)
	db.SetMaxIdleConns(400)
	db.SetConnMaxLifetime(time.Minute * 30)

	if err != nil {
		log.Panic(err)
	}

	if err = db.Ping(); err != nil {
		log.Panic(err)
	}
}

func InsertUser(username string, pw string, status string, bonus int, admin bool) bool {

	var exists bool
	err := db.QueryRow("SELECT EXISTS(SELECT * FROM users WHERE name = ?)", username).Scan(&exists)

	if err != nil || exists {

		return false
	}

	insert, err := db.Query("INSERT IGNORE INTO users (name, password, status, bonus, admin) VALUES (?, ?, ?, ?, ?)", username, pw, status, bonus, admin)
	if err != nil {
		return false
	}
	defer insert.Close()

	return true
}

func AuthUser(username string, pw string) bool {

	var userReq User
	err := db.QueryRow("SELECT password FROM users WHERE BINARY name = ?", username).Scan(&userReq.Password)

	if err != nil {
		return false
	}

	if pw != userReq.Password {
		return false
	}

	return true
}

func UpdateUser(id int, username string, pw string, status string, bonus int, admin bool) {

	update, err := db.Query("UPDATE users SET name=?, password=?, status=?, bonus=?, admin=? WHERE id = ?", username, pw, status, bonus, admin, id)
	if err != nil {
		return
	}
	defer update.Close()

	return
}

func DeleteUser(username string) bool {

	delete, err := db.Query("DELETE FROM users WHERE BINARY name = ?", username)
	if err != nil {
		return false
	}
	defer delete.Close()

	return true
}

func GetUser(username string) User {

	var userReq User
	err := db.QueryRow("SELECT id, name, password, status, bonus, admin FROM users WHERE name = ?", username).Scan(&userReq.ID, &userReq.Username, &userReq.Password, &userReq.Status, &userReq.Bonus, &userReq.Admin)

	if err != nil {
		return User{}
	}
	return userReq
}

func GetUsers() []User {

	results, err := db.Query("SELECT * FROM users ORDER BY id DESC LIMIT 100")

	if err != nil {
		return []User{}
	}

	var users []User

	for results.Next() {
		var user User
		err = results.Scan(&user.ID, &user.Username, &user.Password, &user.Status, &user.Bonus, &user.Admin)
		if err != nil {
			return []User{}
		}
		users = append(users, user)
	}
	return users
}

func AddMessage(username string, sender string, hash string, content string) bool {

	insert, err := db.Query("INSERT INTO messages VALUES (?, ?, ?, ?)", username, sender, hash, content)
	if err != nil {
		return false
	}
	defer insert.Close()

	return true
}

func GetMessages(username string) []Message {

	results, err := db.Query("SELECT name, sender, hash, message FROM messages WHERE name = ?", username)

	if err != nil {
		return []Message{}
	}

	var messages []Message

	for results.Next() {
		var msg Message
		err = results.Scan(&msg.To, &msg.From, &msg.Hash, &msg.Content)
		if err != nil {
			return []Message{}
		}
		messages = append(messages, msg)
	}
	return messages
}

func GetAllMessages(hash string) []Message {

	results, err := db.Query("SELECT name, sender, hash, message FROM messages WHERE hash = ? LIMIT 50", hash)

	if err != nil {
		return []Message{}
	}

	var messages []Message

	for results.Next() {
		var msg Message
		err = results.Scan(&msg.To, &msg.From, &msg.Hash, &msg.Content)
		if err != nil {
			return []Message{}
		}
		messages = append(messages, msg)
	}
	return messages
}

func AddComment(username string, product string, content string) bool {

	insert, err := db.Query("INSERT INTO comments (name, product, content) VALUES (?, ?, ?)", username, product, content)
	if err != nil {
		return false
	}
	defer insert.Close()

	return true
}

func GetComments(product string) []Comment {

	results, err := db.Query("SELECT created_at, name, product, content FROM comments WHERE product = ? ORDER BY id DESC LIMIT 100", product)

	if err != nil {
		return []Comment{}
	}

	var comments []Comment

	for results.Next() {
		var cmnt Comment
		err = results.Scan(&cmnt.Timestamp, &cmnt.User, &cmnt.Product, &cmnt.Content)
		if err != nil {
			return []Comment{}
		}
		comments = append(comments, cmnt)
	}
	return comments
}

func AddTicket(username string, subject string, hash string) bool {

	insert, err := db.Query("INSERT INTO tickets VALUES (?, ?, ?)", username, subject, hash)
	if err != nil {
		return false
	}
	defer insert.Close()

	return true
}

func GetTicket(hash string) Ticket {

	var ticket Ticket
	err := db.QueryRow("SELECT name, subject, hash FROM tickets WHERE hash = ?", hash).Scan(&ticket.User, &ticket.Subject, &ticket.Hash)

	if err != nil {
		return Ticket{}
	}
	return ticket
}

func GetTickets(username string) []Ticket {

	results, err := db.Query("SELECT name, subject, hash FROM tickets WHERE name = ? LIMIT 10", username)

	if err != nil {
		return []Ticket{}
	}

	var tickets []Ticket

	for results.Next() {
		var ticket Ticket
		err = results.Scan(&ticket.User, &ticket.Subject, &ticket.Hash)
		if err != nil {
			return []Ticket{}
		}
		tickets = append(tickets, ticket)
	}

	return tickets
}

func AddOrder(username string, itemID int, color string, quantity int, hash string) bool {

	insert, err := db.Query("INSERT INTO orders (name, itemID, color, quantity, hash) VALUES (?, ?, ?, ?, ?)", username, itemID, color, quantity, hash)
	if err != nil {
		return false
	}
	defer insert.Close()

	return true
}

func GetOrders(username string) []Order {

	results, err := db.Query("SELECT name, itemID, color, quantity, hash FROM orders WHERE name = ? LIMIT 10", username)

	if err != nil {
		return []Order{}
	}

	var orders []Order

	for results.Next() {
		var order Order
		err = results.Scan(&order.Username, &order.ItemID, &order.Color, &order.Quantity, &order.Hash)
		if err != nil {
			return []Order{}
		}
		orders = append(orders, order)
	}

	return orders
}

func GetOrder(hash string) Order {

	var order Order
	err := db.QueryRow("SELECT id, name, itemID, color, quantity, hash FROM orders WHERE hash = ? ORDER BY id DESC", hash).Scan(&order.ID, &order.Username, &order.ItemID, &order.Color, &order.Quantity, &order.Hash)

	if err != nil {
		return Order{}
	}
	return order
}
