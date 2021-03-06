g = {};

var createEnum = function(list){
	var ret = {}
	for(var i=0;i<list.length;i++){
		ret[list[i]] = list[i];
	}
	return ret;
}

var nt = createEnum(["STMTS", "STMTEND", "STMTENDEND", "STMT", "ASSIGN", "ADDEXPR", "ADDEXPRA", "IADDEXPR", "IADDEXPRA", "VAR", "LISTEXPR", "ITEMS", "ITEMEND","ITEM"])
var t = createEnum(["EMPTY", "ENDOFFILE", "SET", "SEMI", "CHAR", "STR", "INT", "REAL", "CAR", "CDR", "PLUS", "LPAREN", "RPAREN"])

g[nt.STMTS] = [[nt.STMT, nt.STMTEND]]
g[nt.STMTEND] = [[nt.STMT, nt.STMTENDEND], [t.EMPTY]]
g[nt.STMTENDEND] = [[nt.STMTEND], [t.ENDOFFILE]]
g[nt.STMT] = [[nt.ASSIGN], [nt.ADDEXPR]]
g[nt.ASSIGN] = [[t.SET, nt.VAR, nt.ADDEXPR]]
g[nt.ADDEXPR] = [[nt.LISTEXPR, nt.ADDEXPRA]]
g[nt.ADDEXPRA] = [[t.PLUS, nt.LISTEXPR, nt.ADDEXPRA], [t.SEMI]]
g[nt.IADDEXPR] = [[nt.LISTEXPR, nt.IADDEXPRA]]
g[nt.IADDEXPRA] = [[t.PLUS, nt.LISTEXPR, nt.IADDEXPRA], [nt.ITEMEND]]
g[nt.LISTEXPR] = [[nt.VAR],[t.LPAREN, nt.ITEMS], [t.CDR, nt.LISTEXPR]]
g[nt.VAR] = [[t.CHAR]]
g[nt.ITEMS] = [[nt.ITEM, nt.ITEMEND], [nt.IADDEXPR, nt.ITEMEND], [t.RPAREN]]
g[nt.ITEMEND] = [[nt.ITEM, nt.ITEMEND], [nt.IADDEXPR], [t.RPAREN]]
g[nt.ITEM] = [[t.CAR, nt.LISTEXPR], [t.STR], [t.INT], [t.REAL]]

// var nt = createEnum(["Z", "X", "Y"])
// var t = createEnum(["a", "c", "d", "EMPTY"])

// g[nt.Z] = [[t.d], [nt.X, nt.Y, nt.Z]]
// g[nt.Y] = [[t.c], [t.EMPTY]]
// g[nt.X] = [[nt.Y], [t.a]]

var firstOf = function(x, done){
	var ret = []
	if(!done){
		done = []
	}
	done.push(x)
	if(x in t){
		return [x];
	}
	for(var i = 0;i<g[x].length;i++){
		for(var j = 0;j<g[x][i].length;j++){
			if(done.indexOf(g[x][i][j]) == -1){
				var f = firstOf(g[x][i][j], done);
				ret = ret.concat(filterEmpty(f));
				if(f.indexOf(t.EMPTY) == -1){
					break;
				}else{
					if(j == g[x][i].length - 1){
						ret.push(t.EMPTY);
					}
				}
			}
		}
	}
	ret = makeUniq(ret)
	return ret;
}

var followOf = function(x, done){
	var ret = []
	if(!done){
		done = []
	}
	done.push(x)
	for(var key in g){
		for(var i = 0;i<g[key].length;i++){
			for(var j = 0;j<g[key][i].length;j++){
				if(g[key][i][j] == x){
					var k = j+1
					while(1){
						if(k == g[key][i].length){
							if(done.indexOf(key) == -1){
								var f = followOf(key, done);
								ret = ret.concat(f);
							}
							break;
						}
						var f = firstOf(g[key][i][k]);
						var filtered = filterEmpty(f);
						ret = ret.concat(filtered);
						if(f.indexOf(t.EMPTY) == -1){
							break;
						}
						k++
					}
				}
			}
		}
	}
	ret = makeUniq(ret);
	return ret;
}

var makeUniq = function(ar){
	var ret = [];
	for(var i = 0;i<ar.length;i++){
		if(ret.indexOf(ar[i]) == -1){
			ret.push(ar[i])
		}
	}
	return ret;
}

var filterEmpty = function(ar){
	var ret = [];
	for(var i = 0;i<ar.length;i++){
		if(ar[i] != t.EMPTY){
			ret.push(ar[i])
		}
	}
	return ret;
}

// for(var key in g){
// 	console.log(key + ">"+ firstOf(key) +">"+followOf(key));
// }

var pt = [];
for(var nonTerm in nt){
	pt[nonTerm] = []
	for(var term in t){
		if(term != t.EMPTY){
			pt[nonTerm][term] = [];
		}
	}
}

var addToPt = function(sym){
		var first = firstOf(sym)
		for(var i = 0;i<first.length;i++){
			if(first[i] != t.EMPTY){
				for(var key in g){
					for(var j = 0;j<g[key].length;j++){
						for(var k = 0;k<g[key][j].length;k++){
							if(g[key][j][k] == sym){
								pt[key][first[i]].push(g[key][j])
							}
							//ONLYS TARTING WITH SYM
							if(firstOf(g[key][j][k]).indexOf(t.EMPTY) == -1){
								break;
							}
						}
					}
				}
			}else{
				//console.log(sym + "hit");
				for(var key in g){
					for(var j = 0;j<g[key].length;j++){
						for(var k = g[key][j].length-1;k>=0;k--){
							if(g[key][j][k] == sym){
								var follow = followOf(key);
								for(var m = 0;m<follow.length;m++){
									// if(key + " " + follow[m]=="ITEM CHAR"){
									// 	console.log(sym, "BAD", g[key][j])
									// }
									pt[key][follow[m]].push(g[key][j])
								}
							}
							//ONLY ENDING WITH SYM which is why k-- above
							if(firstOf(g[key][j][k]).indexOf(t.EMPTY) == -1){
								break;
							}
						}
					}
				}
			}
		}
}

for(var nonTerm in nt){
	addToPt(nonTerm)
}

for(var nonTerm in t){
	//if(nonTerm != t.EMPTY){
		addToPt(nonTerm)
	//}
}
var count = 0;
// for(var key in pt){
// 	var ar = makeUniq(pt[key]);
	
// 	if(ar.length > 0){
// 		console.log(key)
// 		console.log(ar)
// 		count++;
// 	}
// }


for(var nonTerm in t){
	if(term != t.EMPTY){
		process.stdout.write("+"+nonTerm);
	}
}
console.log();
for(var nonTerm in nt){
	process.stdout.write(nonTerm);
	for(var term in t){
		if(term != t.EMPTY){
			process.stdout.write("+"+pt[nonTerm][term]);
		}
	}
	console.log();
}


//console.log(count)