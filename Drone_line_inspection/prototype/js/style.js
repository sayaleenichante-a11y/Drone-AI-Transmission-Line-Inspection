const exploreBtn=document.getElementById("exploreBtn");
exploreBtn.addEventListener("click",function(){
    document.getElementById("about").scrollIntoView({
        behavior:"smooth"
    });
});
