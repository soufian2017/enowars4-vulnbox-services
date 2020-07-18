package routes

import (
	"buggy/go/db"
	"crypto/sha256"
	"encoding/gob"
	"encoding/hex"
	"fmt"
	"html/template"
	"math/big"
	"net/http"
	"strconv"
	"strings"
	"time"

	"github.com/gorilla/mux"
	"github.com/gorilla/securecookie"
	"github.com/gorilla/sessions"
)

type account struct {
	User     db.User
	Auth     bool
	Messages []db.Message
	Orders   []db.Order
	Tickets  []db.Ticket
}

type orderpage struct {
	Order db.Order
}

type productpage struct {
	Account  account
	Comments []db.Comment
}

type ticketpage struct {
	Account account
	Ticket  db.Ticket
}

type userpage struct {
	Account account
	User    []db.User
}

type reg struct {
	Duplicate bool
	Error     bool
}

type login struct {
	Incorrect bool
	Error     bool
}

type result struct {
	User  db.User
	Error error
}

var store *sessions.CookieStore

var tpl *template.Template

func init() {
	authKey := securecookie.GenerateRandomKey(64)
	encryptionKey := securecookie.GenerateRandomKey(32)
	store = sessions.NewCookieStore(
		authKey,
		encryptionKey,
	)
	store.Options = &sessions.Options{
		MaxAge:   60 * 15,
		HttpOnly: true,
	}
	gob.Register(account{})
	tpl = template.Must(template.ParseGlob("templates/*.gohtml"))
}

func Index(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	if acc.Auth {
		tpl.ExecuteTemplate(w, "index.gohtml", acc)
	} else {
		tpl.ExecuteTemplate(w, "index.gohtml", nil)
	}
}

func ProductOne(w http.ResponseWriter, req *http.Request) {
	productPage(w, req, "super")
}

func ProductTwo(w http.ResponseWriter, req *http.Request) {
	productPage(w, req, "mega")
}

func Register(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	if acc.Auth {
		http.Redirect(w, req, "/", http.StatusFound)
	} else {
		if req.Method == http.MethodPost {
			username := req.FormValue("username")
			password := req.FormValue("pw")
			if username != "" && password != "" && username != "buggy-team" {
				insert := db.InsertUser(username, password, "", 0, false)
				if insert {
					sendBonus(username)
					sendWelcome(username)
					redirectOnSuccess(username, session, w, req)
				} else {
					tpl.ExecuteTemplate(w, "register.gohtml", reg{true, false})
				}
			} else {
				tpl.ExecuteTemplate(w, "register.gohtml", reg{false, true})
			}
		} else {
			tpl.ExecuteTemplate(w, "register.gohtml", nil)
		}
	}
}

func User(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	vars := mux.Vars(req)
	user := vars["user"]
	if acc.Auth {
		users := db.GetUsers()
		validUser := users[:0]
		for _, u := range users {
			if keepUser(u, acc.User.Username, user) {
				validUser = append(validUser, u)
			}
		}
		page := userpage{}
		page.User = validUser
		tpl.ExecuteTemplate(w, "user.gohtml", page)
	} else {
		http.Redirect(w, req, "/", http.StatusFound)
	}
}

func Login(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	if acc.Auth {
		http.Redirect(w, req, "/", http.StatusFound)
	} else {
		if req.Method == http.MethodPost {
			username := req.FormValue("username")
			password := req.FormValue("pw")
			if username != "" && password != "" {
				loginValid := db.AuthUser(username, password)
				if loginValid {
					redirectOnSuccess(username, session, w, req)
				} else {
					err = session.Save(req, w)
					if err != nil {
						http.Error(w, err.Error(), http.StatusInternalServerError)
						return
					}
					tpl.ExecuteTemplate(w, "login.gohtml", login{true, false})
				}
			} else {
				tpl.ExecuteTemplate(w, "login.gohtml", login{false, true})
			}
		} else {
			tpl.ExecuteTemplate(w, "login.gohtml", nil)
		}
	}
}

func Logout(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	session.Values["account"] = account{}
	session.Options.MaxAge = -1
	err = session.Save(req, w)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	http.Redirect(w, req, "/", http.StatusFound)
}

func Profile(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	if acc.Auth {
		if req.Method == http.MethodPost {
			req.ParseForm()
			if req.Form["status"] != nil {
				db.UpdateUser(acc.User.ID, acc.User.Username, acc.User.Password, req.FormValue("status"), acc.User.Bonus, acc.User.Admin)
			} else {
				http.Redirect(w, req, "/profile", http.StatusFound)
			}
			http.Redirect(w, req, "/profile", http.StatusFound)
		} else {
			user := db.GetUser(acc.User.Username)
			messages := db.GetMessages(acc.User.Username)
			orders := db.GetOrders(acc.User.Username)
			tickets := db.GetTickets(acc.User.Username)
			acc.User = user
			acc.Messages = messages
			acc.Orders = orders
			acc.Tickets = tickets
			tpl.ExecuteTemplate(w, "profile.gohtml", acc)
		}
	} else {
		http.Redirect(w, req, "/", http.StatusFound)
	}
}

func Order(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	vars := mux.Vars(req)
	hash := vars["hash"]
	account := getAccount(session)
	if account.Auth {
		if len(hash) == 64 {
			order := db.GetOrder(hash)
			if order == (db.Order{}) {
				http.Redirect(w, req, "/", http.StatusFound)
			} else {
				page := orderpage{}
				page.Order = order
				tpl.ExecuteTemplate(w, "order.gohtml", page)
			}
		} else {
			http.Redirect(w, req, "/", http.StatusFound)
		}
	} else {
		http.Redirect(w, req, "/", http.StatusFound)
	}
}

func Ticket(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	if acc.Auth {
		if req.Method == http.MethodPost {
			subject := req.FormValue("subject")
			message := req.FormValue("message")
			if subject != "" && message != "" {
				h := hash(acc.User.Username, strconv.FormatInt(time.Now().Unix(), 10)[:9])
				db.AddMessage("buggy-team", acc.User.Username, h, message)
				db.AddTicket(acc.User.Username, subject, h)
				db.AddMessage(acc.User.Username, "buggy-team", h, "Please be aware that the Buggy Store(tm) Team is really busy right now. Replies might be delayed.")
				http.Redirect(w, req, fmt.Sprintf("/tickets/%s", h), http.StatusFound)
			} else {
				tpl.ExecuteTemplate(w, "ticket.gohtml", acc)
			}
		} else {
			tpl.ExecuteTemplate(w, "ticket.gohtml", acc)
		}
	} else {
		http.Redirect(w, req, "/", http.StatusFound)
	}
}

func Tickets(w http.ResponseWriter, req *http.Request) {
	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	vars := mux.Vars(req)
	hash := vars["hash"]
	account := getAccount(session)
	if account.Auth {
		if len(hash) == 64 {
			messages := db.GetAllMessages(hash)
			if len(messages) < 1 {
				http.Redirect(w, req, "/", http.StatusFound)
			} else {
				account.Messages = messages
				page := ticketpage{}
				page.Account = account
				page.Ticket = db.GetTicket(hash)
				tpl.ExecuteTemplate(w, "tickets.gohtml", page)
			}
		} else {
			http.Redirect(w, req, "/", http.StatusFound)
		}
	} else {
		http.Redirect(w, req, "/", http.StatusFound)
	}
}

func getAccount(s *sessions.Session) account {
	val := s.Values["account"]
	var acc = account{}
	acc, ok := val.(account)
	if !ok {
		return account{Auth: false}
	}
	return acc
}

func redirectOnSuccess(username string, session *sessions.Session, w http.ResponseWriter, req *http.Request) {
	user := db.GetUser(username)
	accountAuth := &account{
		User: user,
		Auth: true,
	}
	session.Values["account"] = accountAuth
	err := session.Save(req, w)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	http.Redirect(w, req, "/", http.StatusFound)
}

func sendError(username string) {
	db.AddMessage(username, "buggy-team", "private", "There was an unexpected error with your welcome bonus, please get in contact with an admin!")
}

func sendWelcome(username string) {
	db.AddMessage(username, "buggy-team", "private", fmt.Sprintf("Welcome %s to the one and only Buggy Store, enjoy your stay!", username))
}

func sendPreorder(username string, buggy string) {
	db.AddMessage(username, "buggy-team", "private", fmt.Sprintf("Thank you for preordering the %s! We will inform you when it becomes available ASAP.", buggy))
}

func productPage(w http.ResponseWriter, req *http.Request, buggy string) {
	buggy = strings.ToLower(buggy)

	session, err := store.Get(req, "buggy-cookie")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	acc := getAccount(session)
	page := productpage{}
	page.Account = acc
	comments := db.GetComments(buggy + "-buggy")
	page.Comments = sortComments(comments, acc.User.Username)
	if acc.Auth {
		if req.Method == http.MethodPost {
			req.ParseForm()
			if req.Form["comment"] != nil {
				db.AddComment(acc.User.Username, buggy+"-buggy", req.FormValue("comment"))
				comments := db.GetComments(buggy + "-buggy")
				page.Comments = comments
			} else {
				quantity, err := strconv.Atoi(req.FormValue("quantity"))
				if err == nil {
					color := req.FormValue("color")
					if isValidColor(color) && quantity > 0 && quantity <= 99 {
						itemID := getItemID(buggy)
						if itemID > 0 {
							h := hash(color, strconv.Itoa(itemID), strconv.Itoa(quantity))
							db.AddOrder(acc.User.Username, itemID, color, quantity, h)
							sendPreorder(acc.User.Username, strings.Title(buggy)+" Buggy")
						}
					}
				}
			}
			http.Redirect(w, req, "/"+buggy+"-buggy", http.StatusFound)
		} else {
			tpl.ExecuteTemplate(w, buggy+"-buggy.gohtml", page)
		}
	} else {
		tpl.ExecuteTemplate(w, buggy+"-buggy.gohtml", page)
	}
}

func sortComments(comments []db.Comment, username string) []db.Comment {
	commentsOther := []db.Comment{}
	commentsUser := []db.Comment{}
	for _, c := range comments {
		if c.User == username {
			commentsUser = append(commentsUser, c)
		} else {
			commentsOther = append(commentsOther, c)
		}
	}
	return append(commentsUser, commentsOther...)
}

func sendBonus(username string) {
	channel := make(chan result)
	go func() {
		users := db.GetUsers()
		for _, u := range users {
			if u.Username == username {
				u.Bonus = 3
				if big.NewInt(int64(u.ID)).ProbablyPrime(0) {
					u.Bonus = 5
				}
				channel <- result{User: u, Error: nil}
				return
			}
		}
	}()
	select {
	case res := <-channel:
		if res.Error == nil {
			db.UpdateUser(res.User.ID, res.User.Username, res.User.Password, res.User.Status, res.User.Bonus, res.User.Admin)
		}
	case <-time.After(1 * time.Second):
		go sendError(username)
	}
}

func keepUser(user db.User, usernameSession string, usernameURL string) bool {
	if usernameSession != "" && (user.Username != usernameSession || usernameURL != user.Username) {
		return false
	}
	return true
}

func hash(strings ...string) string {
	b := make([]byte, 64)
	for _, s := range strings {
		for j := 0; j < 64; j++ {
			b[j] = (s[((j+1)%len(s))] ^ s[(j%len(s))]) ^ b[j]
		}
	}
	sha := sha256.Sum256(b)
	h := hex.EncodeToString(sha[:])
	return h
}

func isValidColor(category string) bool {
	switch category {
	case
		"cyber-cyan",
		"terminal-turquoise",
		"buggy-blue":
		return true
	}
	return false
}

func getItemID(buggy string) int {
	switch buggy {
	case "super":
		return 1
	case "mega":
		return 2
	default:
		return -1
	}
}
