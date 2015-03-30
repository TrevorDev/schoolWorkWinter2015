var shuffle = require('shuffle')
module.exports = function(name){
    var deck = shuffle.shuffle()
    var userCount = 0
    var users = {}
    var cardsInPlay = []
    var currentWinner = null
    this.addUser = function(socket){
        socket.join(name)
        socket.data.room = this
        users[socket.id] = socket
        userCount++
    }
    this.playCard = function(card, socket){
        if(cardsInPlay.length == 0){
            currentWinner = socket
        }else{
            var maxCard = cardsInPlay[0]
            cardsInPlay.forEach(function(c){
                if(c.suit == maxCard.suit && c.sort > maxCard.sort){
                    maxCard = c
                }
            })
            if(card.suit == maxCard.suit && card.sort > maxCard.sort){
                currentWinner = socket
            }
        }
        cardsInPlay.push(card)
        return currentWinner
    }
    this.getCardsInPlay = function(){
        return cardsInPlay
    }
    this.getCardsInPlayValue = function(){
        var value = 0

        cardsInPlay.forEach(function(c){
            console.log(c)
            if(c.suit == "Heart"){
                value++
            }else if(c.suit == "Spade" && c.description == "Queen"){
                value+= 13
            }
        })
        return value
    }
    this.clearCardsInPlay = function(){
        cardsInPlay = []
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
    this.getScores = function(){
        var ret = []
        for(var id in users){
            ret.push(users[id].data.score)
        }
        return ret
    }

    this.getCurScores = function(){
        var ret = []
        for(var id in users){
            ret.push(users[id].data.curMatchScore)
        }
        return ret
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
            if(i.description == "Two" && i.suit == "Club"){
                found  = true
            }
        })
        return found
    }
    this.allHandsPlayed = function(){
        var played = true
        for(var id in users){
            if(users[id].data.hand.length > 0){
                played = false
            }
        }
        return played
    }
    this.shuffle = function(){
        deck = shuffle.shuffle()
    }
}