<h1>MatchmakingSwitch</h1>
[EXPERIMENTAL] This is a tool can help you create no randoms session<br>
<br>
<h4>What it can do & How it work</h4>
This tool can turn off in game matchmaking service.<br>
With this ability it can create a solo public session that random players won't start to join.<br>
Friends and their friends and crew members are free to join but except randoms.<br>
Just click red button in the middle or use hot key (default disabled) it will turn off in game matchmaking service.<br>
<br>
<a href="https://raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/MatchmakingSwitch.exe"><ins>Download here...</ins></a><br>
<a href="//raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/ffmpeg/MatchmakingSwitch.exe"><ins>Download this version if the notification sound is not loud enough...</ins></a><br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch.png"><br>
<!--<a href="https://www.virustotal.com/gui/file/3c5d6335c52a2333999e3b0c711977bf60850a8da0bfd1217358f98409f97d5a/detection" target="_blank"><ins>See virus scan result</ins></a><br>-->
<!--<del>[FIXED] it may look different if the scale and layout setting on Windows 10 is not set to 100%</del>
<br>-->
<h2>How to create a No Randoms Session:</h2>
<!--<h5>Method 1 [RECOMMAND]</h5>-->
<ul>
  <li>Join a Public Session or Find a new session with matchmaking off.</li>
  <li>It should be a empty session (usually it will join a friend, to avoid this enable <a href="/README.md#firewall-solo-session">Firewall Solo Session</a>, then repeat last step, Remember to set it back later).</li><!--(usually it will join a friend, to avoid this set Presence Visibility to Me Only in Social Club setting, Remember to set it back later)-->
  <li>Done. Randoms will not join this session.</li>
</ul>
<!--<h5>Method 2</h5>--><!--[DO NOT RECOMMAND] [If turn matchmaking back on, sometimes will cause randoms to join]-->
<!--<ul>
  <li>Join a public session with other players in it and with matchmaking on.</li>
  <li>Make a solo session and after that wait for few seconds turn matchmaking off.</li>
  <li>Done. Randoms will not join this session.</li>
</ul>-->
<!--<h5>Method 2 [Do Not Recommand. Friends may unable to join through Social Club (No join option)]</h5>
<ul>
  <li>Join a invite only session with matchmaking on.</li>
  <li>Turn matchmaking off then find a new session and the new session should be a empty session.</li>
  <li>Done. Randoms will not join this session.</li>
</ul>-->
<h3>Firewall Solo Session:</h3>
<ul>
  <li>Enable this option to block anyone from joining and disconnect anyone joined. Friends can't join while this is enabled</li>
  <li>This feature require system firewall enabled.
    <details>
      <summary>See this option and how to enable system firewall</summary>
      <br>
      <img src="https://raw.githubusercontent.com/Barracuda10/others/master/MatchmakingSwitch/matchmakingswitch_firewallsolosession.png" width=267 height=216><br>
      Open System and Security->Windows Defender Firewall in Control Panel<br>
      Choose Turn Windows Defender Firewall on or off<br>
      Turn both network settings to on<br>
    </details>
  </li>
</ul>
<br>
Usage is also included inside it's manual.<br>
<details>
  <summary>See how to find manual</summary>
  <br>
  <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_manual.png" width=267 height=216>
</details>
<br>
<h3>Note:</h3>
<ul>
  <li>Turn matchmaking back on before hosting a job or heist that require to launch session.<br>
  <li>This program may require run as administrator.<br>
    <details>
      <summary>See how to run as administrator</summary>
      <br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Right click MatchmakingSwitch.exe, choose Properties, then choose Compatibility tab,<br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Check this option showed below.<br>
      <br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/administrator.png"><br>
    </details>
  </li>
  <li>Due to game default set matchmaking on, this program will automatically turn matchmaking back on while starting it and closing it.</li>
  <li>This program will not work when using VPN or proxy.</li>
  <li>If did correct steps but randoms still join while matchmaking service is off, please click Options->Edit Hosts File, it will pop up a text file, <br>Then check codes in that text file look exactly same to the following codes in picture showed.<br>
    <details>
      <summary>See that picture and how to fix it</summary>
      <br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Find and verify this code: "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>
      <!--&nbsp;&nbsp;-&nbsp;&nbsp;Make sure there is no "#" in front of it, if there is, delete all the "#" sign in the line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>-->
      &nbsp;&nbsp;-&nbsp;&nbsp;<!--Or just simply-->Delete the entire line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com", then restart this program. It will automatically add a new one when starting it<br>
      <br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/hosts_enabled.png"><br>
    </details>
  </li>
  <li>This program is based on network so it won't modified game play and game files.</li>
  <li>This program do not modified any other file except the one showed below.<br>
    <details>
      <summary>Click to see file will be modified by this tool</summary>
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
2&nbsp;&nbsp;If turn matchmaking back on it will immidiately cause randoms to join sometimes, and even if turn it back off quickly<br>
Therefore it's better to never turn it back on until leave this session.<br>
<br>
<!--3&nbsp;&nbsp;If there is already more than 1 people in session and then go turn Matchmaking off, It usually still will cause randoms to join. So the best way to use this is turn it off while only 1 person in the session.<br>-->
<br>
<h2>Advanced features:</h2>
This tool can also disable in game cloud services, make the game unable to connect to the game cloud servers, and it will prevent game to upload and save game progress.<br>
To disable Cloudservices, choose the option show below, it will change to CloudservicesSwitch.<br>
<br>
Just like MatchmakingSwitch, click red button or use hotkey (default disabled) to disable Cloudservices.<br>
Turn off Cloudservices can make the game temporarily unable to save game progress.<br>
To discard this unsaved game data disconnect internet and let the game kick to story mode.<br>
To upload and save this unsaved game data just turn it back on.<br>
<br>
Due to game default set cloudservices on, this program will automatically turn cloudservices back on when starting it and closing it.<br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_cloudservicesswitch.png"><br>
Change back to MatchmakingSwitch anytime through options in picture showed above.<br>
Also it will display each services state if it's On or Off in prompt labal.<br>
