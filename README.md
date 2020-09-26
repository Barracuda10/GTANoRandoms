<h1>MatchingSwitch</h1>
[EXPERIMENTAL] This is a tool to create no randoms session<br>
<br>
<details>
  <summary>What it can do and how it works</summary>
  <br>
  Usually a session will join random people because it's connected to a game server (matchmaking server) that matching random players into the session.<br>
  This tool can block the server that matching randoms into the session.<br>
  This is how it create a Public Session that random players won't start to join, while friends and their friends and crew members are free to join but except randoms.<br>
</details>
Just click red button in the middle or use hot key (default disabled) and it will block matchmaking server.<br>
<br>
<a href="https://raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/MatchingSwitch.exe"><ins>Download here...</ins></a><br>
<a href="//raw.githubusercontent.com/Barracuda10/MatchmakingSwitch/master/MatchmakingSwitch/x64/Release/ffmpeg/MatchingSwitch.exe"><ins>Download this version if the notification sound is not loud enough...</ins></a><br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch.png"><br>
<!--<a href="https://www.virustotal.com/gui/file/3c5d6335c52a2333999e3b0c711977bf60850a8da0bfd1217358f98409f97d5a/detection" target="_blank"><ins>See virus scan result</ins></a><br>-->
<!--<del>[FIXED] it may look different if the scale and layout setting on Windows 10 is not set to 100%</del>
<br>-->
<h2>STEPS: Create a No Randoms Session</h2>
<!--<h5>Method 1 [RECOMMAND]</h5>-->
<ul>
  <li>Join a Public Session or Find a new session with MatchingSwitch ON.</li>
  <li>It should be a empty session<!-- (usually it will join a friend, to avoid this enable --><!--<a href="/README.md#firewall-solo-session">--><!--Firewall Solo Session--><!--</a>--><!-- , then repeat last step, Remember to disable it later).--></li>
  <ul>
    <li>Usually it will join a friend, to avoid this enable <!--<a href="/README.md#firewall-solo-session">-->Firewall Solo Session<!--</a>-->, then find a new session, it should be a empty session, Remember to disable it later</li>
  </ul>
  <li>Done. Randoms will not join this session.</li>
</ul>
<!--<h5>Method 2</h5>
<ul>
  <li>Join a invite only session with matchmaking on.</li>
  <li>Turn matchmaking off then find a new session and the new session should be a empty session.</li>
  <li>Done. Randoms will not join this session.</li>
</ul>-->
<h4>Assisting feature - Firewall Solo Session</h4>
<ul>
  <li>Enable this to block anyone from joining session and disconnect anyone joined. Friends can't join while this is enabled.</li>
  <li>This feature require system firewall enabled.
    <details>
      <summary>See how to enable Firewall Solo Session</summary>
      <img src="https://raw.githubusercontent.com/Barracuda10/others/master/MatchmakingSwitch/matchmakingswitch_firewallsolosession.png" width=267 height=216><br>
    </details>
  </li>
</ul>
This program need system firewall enabled.<br>
<details>
      <summary>See how to enable system firewall</summary>
      <img src="https://raw.githubusercontent.com/Barracuda10/others/master/MatchmakingSwitch/matchmakingswitch_help_3.png" width=267 height=216><br><br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Choose Open Firewall Settings in Help<br>
      <img src="https://raw.githubusercontent.com/Barracuda10/others/master/NetDisconnector/netdisconnector_fw.png" width=267 height=216><br><br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Choose Turn Windows Defender Firewall on or off<br>
      <img src="https://raw.githubusercontent.com/Barracuda10/others/master/NetDisconnector/netdisconnector_fw_on.png" width=267 height=216><br><br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Turn both network settings to on (If know which network profile is using, only need to turn currently using one on)<br>
</details>
Usage is also included inside it's manual.<br>
<details>
  <summary>See how to find manual</summary>
  <br>
  <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_manual.png" width=267 height=216>
</details>
<br>
<h3>Note:</h3>
<ul>
  <li>Always keep MatchingSwitch ON while still in the session. Because turn it off sometimes can cause randoms to join.</li>
  <li>Turn MatchingSwitch OFF before hosting a job or heist that require to launch session.</li>
  <li>This program might not work when using VPN or proxy.</li>
  <li>Due to game default set matchmaking on, this program will automatically turn MatchingSwitch OFF while starting it and closing it.</li>
  <li>This program default using Mode 1 and it need system firewall enabled, <br>If can not enable system firewall, Use Mode 2 instead. Mode 2 may require run as administrator.<br>
    <details>
      <summary>See how to run as administrator</summary>
      <br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Right click MatchmakingSwitch.exe, choose Properties, then choose Compatibility tab,<br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Check this option showed below.<br>
      <br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/administrator.png"><br>
    </details>
  </li>
  <li>If using Mode 2, did correct steps but randoms still join while MatchingSwitch is ON, please do this:<br>
    <details>
      <summary>See how to fix it</summary>
      <br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Click Options->Edit Hosts File, it will pop up a text file.<br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Then check codes in that text file look exactly same to the following codes in picture showed below.<br>
      <img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/hosts_enabled.png"><br><br>
      &nbsp;&nbsp;-&nbsp;&nbsp;Find and verify this code: "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>
      <!--&nbsp;&nbsp;-&nbsp;&nbsp;Make sure there is no "#" in front of it, if there is, delete all the "#" sign in the line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com".<br>-->
      &nbsp;&nbsp;-&nbsp;&nbsp;<!--Or just simply-->Delete the entire line contain "127.0.0.1 mm-gta5-prod.ros.rockstargames.com" and save, then restart this program. It will automatically add a new one when starting it<br>
      <br>
    </details>
  </li>
  <li>This program is based on network so it won't modified game play or game files. <br>If using Mode 2, It will modified the network setting file showed below.<br>
    <details>
      <summary>Click to see file will be modified by this tool</summary>
      &nbsp;&nbsp;-&nbsp;&nbsp;Hosts&nbsp;&nbsp[PATH]%WINDIR%\system32\drivers\etc\hosts<br>
    </details>
  </li>
</ul>
<!--<h3>Cautions:</h3>-->
<h4>Things unavailable when MatchingSwitch is ON, because game will require to use matchmaking service:</h4>
<ul>
  <li>Host any job or heist that require launch session (Join others doesn't include)</li>
  <li>Make a solo session (It will kick to story mode)</li>
  <li><del>[PROVED AVAILABLE] Join friend through social club</del></li>
  <li><del>[PROVED AVAILABLE] Find a new session</del></li>
  <li><del>[PROVED AVAILABLE] Join online</del></li>
</ul>
<br>
<!--2&nbsp;&nbsp;If turn matchmaking back on it will immidiately cause randoms to join sometimes, and even if turn it back off quickly<br>
Therefore it's better to never turn it back on until leave this session.<br>
<br>-->
<!--3&nbsp;&nbsp;If there is already more than 1 people in session and then go turn Matchmaking off, It usually still will cause randoms to join. So the best way to use this is turn it off while only 1 person in the session.<br>-->
<!--<br>
<h2>Advanced features:</h2>
This tool can also disable in game cloud services, make the game unable to connect to the game cloud servers, and it will prevent game to upload and save game progress.<br>
To disable Cloudservices, choose the option show below, it will change to CloudservicesSwitch.<br>
<br>
Just like MatchingSwitch, click red button or use hotkey (default disabled) to disable Cloudservices.<br>
Turn off Cloudservices can make the game temporarily unable to save game progress.<br>
To discard this unsaved game data disconnect internet and let the game kick to story mode.<br>
To upload and save this unsaved game data just turn it back on.<br>
<br>
Due to game default set cloudservices on, this program will automatically turn cloudservices back on when starting it and closing it.<br>
<br>
<br>
<img src="https://github.com/Barracuda10/others/blob/master/MatchmakingSwitch/matchmakingswitch_cloudservicesswitch.png"><br>
Change back to MatchingSwitch anytime through options in picture showed above.<br>
Also it will display each services state if it's On or Off in prompt labal.<br>-->
