from flask import Flask, render_template, request, url_for, jsonify
import requests
import json
app = Flask(__name__)


@app.route("/", methods=['GET', 'POST'])
def home():
    req = requests.get(
        'https://api.thingspeak.com/channels/1995263/feeds.json?api_key=${YOUR_API_KEY}&results=2 ')
    state1 = req.json()["feeds"][-1]["field1"]
    return render_template("website.html", state=state1)


@app.get("/update")
def update():
    req = requests.get(
        'https://api.thingspeak.com/channels/1995263/feeds.json?api_key=${YOUR_API_KEY}&results=2 ')
    state1 = req.json()["feeds"][-1]["field1"]
    time1 = req.json()["feeds"][-1]["created_at"].split("T")
    hour = time1[1].split(".")[0].split("Z")[0]
    date = time1[0]
    usernameID = req.json()["feeds"][-1]["field2"]
    file = json.load(open("usernames.json"))["users"]
    for i in range(len(file)):
        if file[i]["id"] == usernameID:
            username = file[i]["username"]
            break
    list = [hour, date, username, state1]
    return list


if __name__ == "__main__":
    app.run(debug=True)
