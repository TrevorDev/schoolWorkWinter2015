g = {};

var createEnum = function(list){
	var ret = {}
	for(var i=0;i<list.length;i++){
		ret[list[i]] = list[i];
	}
	return ret;
}

var nt = createEnum(["STMTS", "STMTEND", "STMT", "ASSIGN", "ADDEXPR", "ADDEXPRA", "VAR", "LISTEXPR", "ITEMS", "ITEMEND", "ITEM"])
var t = createEnum(["EMPTY", "SET", "SEMI", "CHAR", "STR", "INT", "REAL", "CAR", "CDR", "PLUS", "LPAREN", "RPAREN"])



g[nt.STMTS] = [[nt.STMT, nt.STMTEND]]
g[nt.STMTEND] = [[nt.STMT, nt.STMTEND], [t.EMPTY]]
g[nt.STMT] = [[nt.ASSIGN, t.SEMI], [nt.ADDEXPR, t.SEMI]]
g[nt.ASSIGN] = [[t.SET, nt.VAR, nt.ADDEXPR]]
g[nt.ADDEXPR] = [[nt.ADDEXPRA, t.PLUS, nt.ADDEXPR], [nt.ADDEXPRA]]
g[nt.ADDEXPRA] = [[nt.LISTEXPR, t.PLUS, nt.ADDEXPRA],[nt.LISTEXPR]]
g[nt.LISTEXPR] = [[nt.VAR],[t.LPAREN, nt.ITEMS, t.RPAREN], [t.CDR, nt.LISTEXPR]]
g[nt.VAR] = [[t.CHAR]]
g[nt.ITEMS] = [[nt.ITEM, nt.ITEMS], [t.EMPTY]]
g[nt.ITEM] = [[nt.ADDEXPR], [t.CAR, nt.LISTEXPR], [t.STR], [t.INT], [t.REAL]]

var firstOf = function(x){
	var ret = []
	if(x in t){
		return [x];
	}
	for(var i = 0;i<g[x].length;i++){
		for(var j = 0;j<g[x][i].length;j++){
			var f = firstOf(g[x][i][j]);
			f = makeUniq(f);
			ret = ret.concat(f);
			if(f.indexOf(t.EMPTY) == -1){
				break;
			}
		}
	}
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

for(var key in g){
	console.log(key);
	console.log(firstOf(key))
}
