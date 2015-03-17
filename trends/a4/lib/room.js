var shuffle = require('shuffle')
module.exports = function(name){
    var deck = shuffle.shuffle()
    var userCount = 0
    var users = {}
    this.addUser = function(socket){
        socket.join(name)
        socket.room = this
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
}