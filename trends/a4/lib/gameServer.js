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
            hand: null,
            score: 0,
            curMatchScore: 0
        }
        console.log('a user connected');
        if(!rooms[currentRoom]){
            gs.addRoom(currentRoom)
        }
        rooms[currentRoom].addUser(socket)
        if(rooms[currentRoom].getUserCount() == 4){
            var u = rooms[currentRoom].getUsers()
            var playerNum = 1;
            for(var id in u){
                u[id].data.hand = rooms[currentRoom].dealHand()
                u[id].emit('gameStart', {hand: u[id].data.hand, playerNum: playerNum++, scores: socket.data.room.getScores()})
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

        socket.on("playCard", function(card){
            var currentWinner = socket.data.room.playCard(socket.data.hand[card.servIndex], socket)
            socket.data.hand = socket.data.hand.filter(function(val, index){
                return index != card.servIndex
            })
            var u = socket.data.room.getUsers()
            var roundDone = socket.data.room.getCardsInPlay().length == 4;
            if(roundDone){
                var setAmt = socket.data.room.getCardsInPlayValue()
                currentWinner.data.score += setAmt
                currentWinner.data.curMatchScore += setAmt
            }
            for(var id in u){
                var yourTurn = socket.data.room.getPlayerOnLeft(socket) == u[id]
                if(roundDone){
                    yourTurn = currentWinner == u[id]
                }
                u[id].emit('cardPlayed', {
                    cardsInPlay: socket.data.room.getCardsInPlay(),
                    hand: u[id].data.hand,
                    yourTurn: yourTurn,
                    scores: socket.data.room.getCurScores()
                })
            }


            if(roundDone){
                socket.data.room.clearCardsInPlay()
            }

            if(socket.data.room.allHandsPlayed()){
                socket.data.room.shuffle()

                for(var id in u){
                    if(u[id].data.curMatchScore == 26){
                        u[id].data.score -= 26*2;
                        for(var id in u){
                            u[id].data.score += 26;
                        }
                        break;
                    }
                }

                var playerNum = 1;
                for(var id in u){
                    u[id].data.curMatchScore = 0
                    u[id].data.hand = socket.data.room.dealHand()
                    u[id].emit('gameStart', {hand: u[id].data.hand, playerNum: playerNum++, scores: socket.data.room.getScores()})
                }
            }

        })
        socket.onclose = function(reason){ 
            //USE instead of disconnect to have access to sockets rooms
            //console.log(socket.rooms)
            Object.getPrototypeOf(this).onclose.call(this,reason);
        }
    });


}



