const express = require('express');
const bodyParser = require('body-parser');
const morgan = require("morgan");
const helmet = require("helmet");

//INSTANTIATE OUR API app
const app = express();

// fix CORS issue
app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header(
        "Access-Control-Allow-Headers",
        "Origin, X-Requested-With, Content-Type, Accept"
    );
    next();
});

// setup middlewares for Body parser
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(morgan("dev"));
app.use(helmet());

// setup the routes / endpoints
const endpoints = require("./endpoints");

// Use the routes
app.use('/', endpoints);

// set up port and listen
const port = process.env.PORT || 3000;

app.listen(port, () => {
    console.log(`SERVER LISTENING ON PORT ${port}`);

})