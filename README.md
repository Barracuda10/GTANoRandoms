<h1>MatchmakingSwitch</h1>
[EXPERIMENTAL] This is a tool can help you create no randoms session<br>
This tool can turn off in game matchmaking service.<br>
Just simply click red button in the middle or use hot key (default disabled) it will turn off in game matchmaking service.<br>
<br>
<a href="https://raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/MatchmakingSwitch.exe"><ins>Download here...</ins></a><br>
<a href="//raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/ffmpeg/MatchmakingSwitch.exe"><ins>Download louder notification sound version here...</ins></a><br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch.png"><br>
<br>
<!--<del>[FIXED] it may look different if the scale and layout setting on Windows 10 is not set to 100%</del>
<br>-->
<h2>How to create a No Randoms Session:</h2>
<h5>Method 1 [If turn matchmaking back on, sometimes will cause randoms to join]</h5>
<ul>
  <li>Join a public session with other players in it and with matchmaking on.</li>
  <li>Make a solo session and after that wait for few seconds turn matchmaking off.</li>
  <li>Turn matchmaking back on before host a job or heist that require launch session.</li>
</ul>
<!--<h5>Method 2 [Do Not Recommand. Friends may unable to join through Social Club (No join option)]</h5>
<ul>
  <li>Join a invite only session with matchmaking on.</li>
  <li>Turn matchmaking off then find a new session and the new session should be a empty session.</li>
  <li>Turn matchmaking back on before host a job or heist that require launch session.</li>
</ul>-->
Usage is also included inside it's manual.<br>
<details>
  <summary>See how to find manual</summary>
  <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_manual.png">
</details>
<br>
<h3>Note:</h3>
<ul>
  <li>This program may require run as administrator.<br>
    <details>
      <summary>Click to see how to run as administrator</summary>
      <br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Right click MatchmakingSwitch.exe, choose Properties, then choose Compatibility tab,<br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Check this option showed below.<br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/administrator.png"><br>
    </details>
  </li>
  <li>Due to game default set matchmaking on, it will automatically turn matchmaking back on while starting it and closing it.</li>
  <li>This program will not work when using VPN or proxy.</li>
  <li>If randoms still join while matchmaking off, please click Options->Edit Hosts File, it will pop up a text file, <br>Then check following codes in picture showed below look exactly same in that text file.<br>
    Hint: Find and verify this code: "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Make sure there is no "#" in front of it, if there is, delete all the "#" sign in front of the line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Or just simply delete entire line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com", then restart this program. It will automatically add a new one when starting it<br>
    <details>
      <summary>Click to see that picture</summary>
      <br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/hosts_enabled.png"><br>
    </details>
  </li>
  <li>This program will not modified any other file except the one showed below. This program is based on network so it won't modified game play.<br>
    <details>
      <summary>Click to see file this tool will be modified</summary>
      &nbsp;&nbsp;-&nbsp;&nbsp;Hosts&nbsp;&nbsp[PATH]%WINDIR%\system32\drivers\etc\hosts<br>
    </details>
  </li>
</ul>
<br>
<h2>Cautions:</h2>
1&nbsp;&nbsp;Things unavailable when matchmaking is off, because game will require to use matchmaking service:<br>
<br>
<ul>
  <li>Host any job or heist that require launch session (Join others doesn't include)</li>
  <li>Make a solo session (It will kick to story mode)</li>
  <li><del>[PROVED AVAILABLE] Join friend through social club</del></li>
  <li><del>[PROVED AVAILABLE] Find a new session</del></li>
  <li><del>[PROVED AVAILABLE] Join online</del></li>
</ul>
<br>
2&nbsp;&nbsp;If turn matchmaking back on it will immidiately cause randoms join sometimes, and even if turn it back off quickly<br>
Therefore it's better to never turn it back on until leave this session.<br>
<br>
3&nbsp;&nbsp;If there is already more than 1 people in session and then go turn Matchmaking off, It usually will still cause randoms to join. So the best way to use this is turn it off while only 1 person in the session.<br>
<br>
<h2>Advanced features:</h2>
This tool can also disable in game cloud services, make the game unable to connect game server, and unable to save game progress.<br>
It is useful if you don't want the game auto save progress.<br>
To disable Cloudservices, choose the option show below, it will change to CloudservicesSwitch.<br>
<br>
Just like MatchmakingSwitch, click red button or use hotkey (default disabled) will disable Cloudservices.<br>
Turn off Cloudservices can make the game temporarily unable to save game data.<br>
To discard this unsaved game data disconnect internet and let the game kick to story mode.<br>
To upload and save this unsaved game data just turn it back on.<br>
<br>
Due to game default set cloudservices on, this program will automatically turn cloudservices back on when starting it and closing it.<br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_cloudservicesswitch.png"><br>
You can also change back to matchmakingswitch anytime, it will display each services state if it's On or Off.<br>
