var Room = require('../lib/room')

module.exports = function(server){
    
    var rooms = {}
    var currentRoom = 0;
    var gs = this

    gs.addRoom = function(r){
        var roomName = r
        rooms[roomName] = new Room(roomName)
    }
//SOCKETIO ---------------------------------------------------------------------------------------------------------------------
    var io = require('socket.io').listen(server);
    io.on('connection', function(socket) {
        socket.data = {
            room: null
        }
        console.log('a user connected');
        if(!rooms[currentRoom]){
            gs.addRoom(currentRoom)
        }
        rooms[currentRoom].addUser(socket)
        if(rooms[currentRoom].getUserCount() == 1){
            var u = rooms[currentRoom].getUsers()
            for(var id in u){
                u[id].emit('gameStart', rooms[currentRoom].dealHand())
            }
            currentRoom++
        }
        
        socket.onclose = function(reason){ 
            //USE instead of disconnect to have access to sockets rooms
            console.log(socket.rooms)
            Object.getPrototypeOf(this).onclose.call(this,reason);
        }
    });


}



