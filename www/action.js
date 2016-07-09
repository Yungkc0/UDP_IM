addLoadEvents(fun);
function addLoadEvents(func){
	oldonload=window.onload;
	if(typeof(oldonload)!="function"){
		window.onload=func;
	}else{
		window.onload=function(){
		oldonload();
		func();
		}
	}
}
function fun(){
	alert(encodeURIComponent("+++--*/*/*546"));
}
funxx();
/*var browserInfo={
	appCodeName:navigator.appCodeName,
	appName:navigator.appName,
	appVersion:navigator.appVersion,
	cookieEnabled:navigator.cookieEnabled,
	platform:navigator.platform,
	userAgent:navigator.userAgent,
	systemLanguage:navigator.systemLanguage
};*/
