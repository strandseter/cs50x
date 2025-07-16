import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session.get('user_id')

    user = db.execute("SELECT * FROM users WHERE id = ?", session.get('user_id'))[0]

    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", user_id)

    total_all = user["cash"]

    for item in portfolio:
        quote = lookup(item["symbol"])

        total = quote["price"] * item["shares"]
        total_all += total

        item["price"] = quote["price"]
        item["total"] = total

    return render_template("index.html", portfolio=portfolio, total=total_all, cash=user['cash'])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol", 400)

        symbol = symbol.upper()

        shares = request.form.get("shares")
        if not shares:
            return apology("must provide shares", 400)

        quote = lookup(symbol)
        if not quote:
            return apology(f"failed to find quote for {symbol}")

        try:
            shares = int(shares)
        except ValueError:
            return apology("shares must be a number", 400)

        if shares < 1:
            return apology("shares must be a positive number", 400)

        user_id = session.get('user_id')

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

        price = shares * quote['price']

        user_cash = user["cash"] - price

        if user_cash < 0:
            return apology("not enough cash")

        portfolio = db.execute(
            "SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", user_id, symbol)

        if len(portfolio) != 1:
            db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES (?, ?, ?)",
                       user_id, symbol, 0)

        portfolio = db.execute(
            "SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]

        updated_shares = portfolio["shares"] + shares

        db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                   updated_shares, user_id, symbol)

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?);",
                   user_id, quote["symbol"], shares, quote["price"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash, user_id)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session.get('user_id')

    history = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY timestamp", user_id)

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol", 400)

        quote = lookup(symbol)

        if not quote:
            return apology(f"failed to find quote for {symbol}")

        return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must provide password again", 400)

        if password != confirmation:
            return apology("passwords not matching")

        hashed_password = generate_password_hash(password)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                       username, hashed_password)
        except ValueError:
            return apology("username must be unique", 400)
        except:
            return apology("something whent wrong registering user", 500)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)
        if not shares:
            return apology("must provide shares", 400)

        symbol = symbol.upper()

        quote = lookup(symbol)
        if not quote:
            return apology(f"failed to find quote for {symbol}")

        try:
            shares = int(shares)
        except ValueError:
            return apology("shares must be a number", 400)

        if shares < 1:
            return apology("shares must be a positive number", 400)

        user_id = session.get('user_id')

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

        portfolio = db.execute(
            "SELECT * FROM portfolio WHERE user_id = ? AND symbol = ? ", user_id, symbol)[0]

        updated_shares = portfolio["shares"] - shares

        if updated_shares < 0:
            return apology("you dont own that many shares", 400)

        if updated_shares > 0:
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                       updated_shares, user_id, symbol)
        else:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", user_id, symbol)

        price = shares * quote['price']

        user_cash = user["cash"] + price

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?);",
                   user_id, quote["symbol"], -shares, quote["price"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash, user_id)

        return redirect("/")
    else:
        user_id = session.get('user_id')

        portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", user_id)

        return render_template("sell.html", portfolio=portfolio)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "POST":
        amount = request.form.get("amount")

        if not amount:
            return apology("must provide amount", 400)

        try:
            amount = int(amount)
        except ValueError:
            return apology("amount must be a number", 400)

        if amount < 1:
            return apology("amount must be a positive number", 400)

        user_id = session.get('user_id')

        user = db.execute("SELECT * FROM users WHERE id = ?", session.get('user_id'))[0]

        updated_cash = user["cash"] + amount

        db.execute("UPDATE users SET cash = ? WHERE id = ?;", updated_cash, user_id)

        return redirect("/")
    else:
        return render_template("deposit.html")
