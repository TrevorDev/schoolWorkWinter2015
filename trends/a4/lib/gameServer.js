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
            room: null,
            leftPlayer: null,
            rightPlayer: null,
            hand: null
        }
        console.log('a user connected');
        if(!rooms[currentRoom]){
            gs.addRoom(currentRoom)
        }
        rooms[currentRoom].addUser(socket)
        if(rooms[currentRoom].getUserCount() == 2){
            var u = rooms[currentRoom].getUsers()
            for(var id in u){
                u[id].data.hand = rooms[currentRoom].dealHand()
                u[id].emit('gameStart', {hand: u[id].data.hand})
            }
            currentRoom++
        }
        socket.on("passCards", function(cards){
            var toPass = []
            cards.forEach(function(i){
                toPass.push(socket.data.hand[i.servIndex])
            })
            var l = socket.data.room.getPlayerOnLeft(socket)
            l.data.hand = l.data.hand.concat(toPass)
            socket.data.hand = socket.data.hand.filter(function(val){
                return toPass.indexOf(val) == -1
            })
            var u = socket.data.room.getUsers()
            for(var id in u){
                if(u[id].data.hand.length != 13){
                    return;
                }
            }
            for(var id in u){
                u[id].emit('receivePass', {hand: u[id].data.hand, yourTurn: u[id].data.room.userHasTwoOfSpades(u[id])})
            }
        })
        socket.onclose = function(reason){ 
            //USE instead of disconnect to have access to sockets rooms
            //console.log(socket.rooms)
            Object.getPrototypeOf(this).onclose.call(this,reason);
        }
    });


}



