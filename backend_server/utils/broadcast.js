function broadcastToClients(wss, msg) {
  wss.clients.forEach((client) => {
    if (client.readyState === 1) client.send(JSON.stringify(msg));
  });
}
module.exports = { broadcastToClients };
