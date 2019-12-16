
<head>
   <link rel="shortcut icon" href="favicon.ico" type="image/x-icon">
   <meta charset="utf-8">
   <meta http-equiv="X-UA-Compatible" content="IE=edge">
   <meta name="description" content="">
   <meta name="keywords" content="">
   <meta name="author" content="">
   <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
   <meta charset="utf-8" />
   <meta name="viewport" content="width=device-width, initial-scale=1" />
   <style>
   </style>
   <link rel="stylesheet" href="assets/css/main.css" />
   <link rel='stylesheet' href='assets/css/bootstrap.min.css'>
   <link rel='stylesheet' href='assets/css/vendor.css'>
   <link rel='stylesheet' href='assets/css/theme_black_red.css' id="theme">
   <link rel='stylesheet' href='assets/css/custom.css'>
   <link rel='stylesheet' href='assets/css/demo.css'>
   <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
   <script src="assets/js/vendor/modernizr-2.8.3.min.js"></script>
   <script src='assets/js/vendor/html5shiv.min.js'></script>
   <link rel="icon" type="image/ico" href="fav.ico"/>
</head>
<body onLoad="scrlsts()">
   <div class="site-loader">
      <span></span>
      <p>Loading Site...</p>
   </div>
   <div class="pmeme">
      <video id="video" autoplay="autoplay" loop="loop" onloadstart="this.volume=0.2">
         <source src="assets/ded.mp4" type="video/mp4"></source>
      </video>
      <embed name="big" src="/assets/big.mp3" loop="true" hidden="true" autostart="true">
   </div>
   <div class="site-bg">
      <div class="site-bg__overlay"></div>
      <div class="site_countdown"></div>
      <div class="site-bg__effect"></div>
      <canvas class="site-bg__canvas"></canvas>
   </div>
   </header>
   <div class="loading">
      <div id="wrapper">
         <div id="main">
            <header id="header">
               <h1>Sour Cheats</h1>
               <h2><font size="2">Quality Cheats!</font></h2>
               <nav>
                  <ul>
                      
                      <li><a href="https://discord.gg/hxXRzsH" class="icon fa-spin fa-comments" target="_blank"><span class="label">Discord</span></a></li>
                     <li><a href="https://sourcheats.selly.store/" class="fa fa-shopping-cart" target="_blank"><span class="label">Store</span></a></li>                      
                  </ul>
               </nav>
            </header>
         </div>
      </div>
   </div>
   <script src="assets/js/vendor/jquery-2.1.4.min.js"></script>
   <script src='assets/js/vendor/bootstrap.min.js'></script>
   <script src='assets/js/vendor/plugin.js'></script>
   <script src='assets/js/variable.js'></script>
   <script src='assets/js/main.js'></script>
   <script src='assets/js/demo.js'></script>
</body>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
   <script language="JavaScript">
      //////////F12 disable code////////////////////////
          document.onkeypress = function (event) {
              event = (event || window.event);
              if (event.keyCode == 123) {
                 //alert('No F-12');
                  return false;
              }
          }
          document.onmousedown = function (event) {
              event = (event || window.event);
              if (event.keyCode == 123) {
                  //alert('No F-keys');
                  return false;
              }
          }
      document.onkeydown = function (event) {
              event = (event || window.event);
              if (event.keyCode == 123) {
                  //alert('No F-keys');
                  return false;
              }
          }
      var message=":o";      
      function clickIE() {if (document.all) {(message);return false;}}
      function clickNS(e) {if
      (document.layers||(document.getElementById&&!document.all)) {
      if (e.which==2||e.which==3) {(message);return false;}}}
      if (document.layers)
      {document.captureEvents(Event.MOUSEDOWN);document.onmousedown=clickNS;}
      else{document.onmouseup=clickNS;document.oncontextmenu=clickIE;}
      document.oncontextmenu=new Function("return false")
      function disableCtrlKeyCombination(e)
      {
      var forbiddenKeys = new Array('a', 'n', 'c', 'x', 'i', 'j' , 'w');
      var key;
      var isCtrl;
      if(window.event)
      {
      key = window.event.keyCode;
      if(window.event.ctrlKey)
      isCtrl = true;
      else
      isCtrl = false;
      }
      else
      {
      key = e.which;
      if(e.ctrlKey)
      isCtrl = true;
      else
      isCtrl = false;
      }
      if(isCtrl)
      {
      for(i=0; i<forbiddenKeys.length; i++)
      {
      if(forbiddenKeys[i].toLowerCase() == String.fromCharCode(key).toLowerCase())
      {
      alert('Key combination CTRL + '+String.fromCharCode(key) +' has been disabled.');
      return false;
      }
      }
      }
      return true;
      }
   </script>
</head>
</html>
<script type="text/javascript">
                var msg  = document.title ="Sour Cheats | Site by bazhar#0069";
                var speed = 150;
                var endChar = " ";
                var pos = 0;
                
                function moveTitle()
                {
                     var ml = msg.length;
                        
                    title = msg.substr(pos,ml) + endChar + msg.substr(0,pos);
                  document.title = title;
                    
                  pos++;
                  if (pos > ml) pos=0;
                  window.setTimeout("moveTitle()",speed);
                }
            
                moveTitle();
            </script>