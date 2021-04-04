const express = require("express");
const router = express.Router();

const si = require('systeminformation');
var cors = require('cors');

// @route   GET /
// @desc    Download using WGET
// @access  Public
router.get("/", (req, res) => {


    si.currentLoad().then(cb => {
        si.networkInterfaces()
            .then(loaddata => {
                console.log('CURRENT LOAD DATA')
                console.log(loaddata);

                console.log('current load is c : ' + cb.currentLoadSystem)

                console.log('current internet speed is : ' + loaddata[0].speed)

                res.status(200).json({

                    //currentload: cb.currentLoad / 10,
                    c: Math.round((cb.currentLoadSystem / 10 + 1)),
                    d: Math.round((loaddata[0].speed / 1000)), // convert speed to MBps

                });
            })

    })

});

module.exports = router;