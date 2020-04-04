const cors = require('cors');
const express = require('express');
const mysql = require('mysql');
const bodyParser = require('body-parser');

const app = express();

const pool = mysql.createPool({
    host: process.env.MYSQL_HOST_IP,
    user: process.env.MYSQL_USER,
    password: process.env.MYSQL_PASSWORD,
    database: process.env.MYSQL_DATABASE,
});

app.use(cors());
app.use(bodyParser.json());

app.listen(process.env.REACT_APP_SERVER_PORT, () => {
    console.log(`App server now listening on port ${process.env.REACT_APP_SERVER_PORT}`);
});

app.get('/test', (req, res) => {

    console.log('Get recieved')

    pool.query('SELECT * FROM `sample` `text`', (err, results) => {
        if (err) {
            return res.send(err);
        } else {
            console.log(results);
            return res.send(results);
        }
    });
});

app.post('/newtext', (req, res) => {

    console.log('Post Recieved');
    console.log(req.body);
    
    pool.query(`INSERT INTO \`sample\`(\`text\`) VALUES ('${req.body.text}')`, (err, results) => {
        if (err) {
            console.log(err);
        }
    })

})