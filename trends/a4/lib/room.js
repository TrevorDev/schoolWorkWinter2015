var shuffle = require('shuffle')
module.exports = function(name){
    var deck = shuffle.shuffle()
    var userCount = 0
    var users = {}
    this.addUser = function(socket){
        socket.join(name)
        socket.data.room = this
        users[socket.id] = socket
        userCount++
    }
    this.dealHand = function(){
    	return deck.draw(13)
    }
    this.getUsers = function(){
    	return users
    }
    this.getUserCount = function(){
    	return userCount
    }
    this.getPlayerOnLeft = function(socket){
        var leftUser = null
        for(var id in users){
            if(id == socket.id){
                if(leftUser != null){
                    return leftUser
                }
            }
            leftUser = users[id]
        }
        return leftUser
    }
    this.userHasTwoOfSpades = function(socket){
        var found = false;
        socket.data.hand.forEach(function(i){
            //console.log(i.sort)
            if(i.sort == 2){
                found  = true
            }
        })
        return found
    }
}