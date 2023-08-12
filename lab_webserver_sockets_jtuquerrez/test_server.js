
const express = require('express');
const path = require('path')

//Port to listen: 3000
const port = 3000;

//Create web application
const app = express();

//Serve static content on www folder
app.use('/', express.static(path.join(__dirname, 'www')))

//Start the server
app.listen(port, () => {
    console.log(`Example server listening on port ${port}`)
  })
