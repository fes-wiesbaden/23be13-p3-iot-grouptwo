const express = require('express');
const router = express.Router();
const { getSensorData } = require('../db/queries');
const { DEVICE_TYPES } = require('../config');

router.get('/', (req, res) => res.send('🌐 WebSocket + MQTT server is working!'));

router.get('/devices', (req, res) => res.json({ supportedTypes: DEVICE_TYPES }));

router.get('/sensors', async (req, res) => {
  try {
    const result = await getSensorData();
    res.json(result.rows);
  } catch {
    res.status(500).send('Internal Server Error');
  }
});

module.exports = router;
