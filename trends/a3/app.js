var config = require('./lib/config')
    //var database = require('./lib/database')
var logger = require('koa-logger')
var router = require('koa-router')
var serve = require('koa-static')
var session = require('koa-generic-session')
var views = require('co-views')
var parse = require('co-body')
var jsonResp = require('koa-json-response')
var koa = require('koa')
var swig = require('swig')
var https = require('https')
var http = require('http')
var request = require('request');
var fs = require('fs')
var passport = require('koa-passport'),
    LocalStrategy = require('passport-local').Strategy;
var app = koa()
var auth = require('./lib/auth.js');
var bodyParser = require('koa-bodyparser')
var session = require('koa-sess')
//Add database
// si = database.getSequelizeInstance()
//si.sync()

//var userCtrl = require('./controller/user')

//REMOVE IN PRODUCTION??
swig.setDefaults(config.templateOptions)
app.keys = ['your-session-secret']
app.use(session())
app.use(bodyParser())
app.use(passport.initialize())
app.use(passport.session())
app.use(jsonResp())
app.use(router(app))

//ROUTES --------------------------------------------------------------------------------------------------------------------

//AUTH
auth.setRoutes(app);

//DEFAULTS
app.get('/', defaultPageLoad('index'))
app.get(/\/public\/*/, serve('.'))

//SECURE
var secured = new router()
app.use(function*(next) {
	console.log(this.session)
  if (this.isAuthenticated()) {
    yield next
  } else {
    this.redirect('/')
  }
})

secured.get('/account',defaultPageLoad('account'))

app.use(secured.middleware())

//API ROUTES
//app.get('/testUser', userCtrl.getUsers)

//PAGE HANDLERS ---------------------------------------------------------------------------------------------------------------------
function defaultPageLoad(pageName, requiresLogin) {
    return function * () {
        /*if(requiresLogin===true && !sessionHelper.isLoggedIn(this.session)){
			this.redirect('/login')
			return
		}*/
        var temp = {};
        this.body = yield render(pageName, temp)
    }
}

function render(page, template) {
    return views(__dirname + '/view', config.templateOptions)(page, template)
}

var server = http.createServer(app.callback())




//SOCKETIO ---------------------------------------------------------------------------------------------------------------------
var io = require('socket.io').listen(server);
function findClientsSocketByRoomId(roomId) {
    var res = []
    , room = io.sockets.adapter.rooms[roomId];
    if (room) {
        for (var id in room) {
        res.push(io.sockets.adapter.nsp.connected[id]);
        }
    }
    return res;
}
var users = {}
var defRoomName = "default"
io.on('connection', function(socket) {
    console.log('a user connected');
    socket.data = {}
    socket.on("login", function(data){
        if(data.name){
            if(users[data.name]){
                socket.emit("login",{error: "bad name"})
            }else{
                users[data.name] = socket
                socket.data.name = data.name

                socket.join(defRoomName);
                socket.broadcast.to(defRoomName).emit('userJoinedRoom', { roomName: defRoomName, userName: socket.data.name });
                var socketsInRoom = findClientsSocketByRoomId(defRoomName);

                socket.emit("login",{name: data.name, room: {name: defRoomName, users: socketsInRoom.map(function(sock){return sock.data.name})}})
            }
        }else{
            socket.emit("login",{error: "bad req"})
        }
    })
    socket.on("createPMRoom", function(data){
        var roomName = "PM_"+data.userName+"-"+socket.data.name;
        socket.join(roomName)
        if(users[data.userName]){
            users[data.userName].join(roomName)
        }
        io.sockets.to(roomName).emit("roomJoined", { room: {name: roomName, users: [data.userName, socket.data.name]} })
    })

    socket.on("sendMsg", function(data){
        console.log(data);
        //socket.broadcast.
        io.sockets.to(data.roomName).emit("sendMsg", { roomName: data.roomName, userName: socket.data.name, msg: data.msg })
    })

    socket.onclose = function(reason){ 
        //HACK to emit left room before socketio deletes all rooms before disconnect msg is received
        //emit to rooms here
        //acceess socket.adapter.sids[socket.id] to get all rooms for the socket
        for(var key in socket.adapter.sids[socket.id]){
            socket.broadcast.to(key).emit('userLeftRoom', { roomName: key, userName: socket.data.name });
        }
        Object.getPrototypeOf(this).onclose.call(this,reason);
    }
    socket.on('disconnect', function() {
        console.log('user disconnected');
        delete users[socket.data.name]
    });
});

server.listen(config.appPort);
console.log('Started ----------------------------------------------' + config.appPort)
