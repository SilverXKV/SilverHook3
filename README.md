# SilverHook
## This is made for 1.16 and auto-updates using sigscanning for it. 1.17 came out and broke it and im too lazy to update.
Internal HOI4 Cheat for Windows
Uses <a href="https://github.com/Rebzzel/kiero">KieroHook</a>, <a href="https://github.com/TsudaKageyu/minhook">MinHook</a> and <a href="https://github.com/ocornut/imgui">ImGui</a>

# Features
### Lobby Cheats
- Hide Steam Name
- Hide HOI4 Name (Join as Ghost does this but better)
- Allow Player to change Custom Game Rules
- Add Fake Players
- Start Games
- Fake Leave Games and Ghost yourself
- Antiban
### In-game Cheats
- Free Templates
- Free Upgrades
- Enable Debug
- Crasher
- Enable and Disable AI on all
- Unpauseable Pause
- Ghost Pause
- Decrease and Increase MP game speed as player
- Autosave MP game as player (Make host autosave)
- Enable / Disable Fog of War
- Enable / Disable Allowtraits
- Enable / Disable Debug
- Tagswitch, Enable or Disable AI on any specific country
### Debug Options
- Boost Yourself
(Only majors can be boosted, to boost yourself go to singleplayer and custom game rules and click any arrow in the boost part of the rules thats on your country, then the country tag under the boost amount should be a bunch of numbers, than you can enter any number preferably larger than 250 to boost your own country in any game for that session)


# ImGui-DirectX-11-Kiero-Hook
Universal ImGui implementation through DirectX 11 Hook (kiero)
<h1>Setting up the solution</h1>
<ul>
  <li>Download and install <a href="https://www.microsoft.com/en-us/download/details.aspx?id=6812">DirectX SDK</a></li>
  <li>Open the solution on Visual Studio and open the project Properties</li>
  <li>Go to VC++ Directories -> Include Directories. Click on 'Edit' and select the Include folder <br/>located on your DirectX SDK installation path. It is generally this one: <br/>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Include\
  <li>Now go to VC++ Directories -> Library Directories. Click on 'Edit' and select the library folder <br/> located on your DirectX SDK installation path. It is generally this one - choose x86 for 32bit and x64 for 64bit: <br/>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Lib\</li>
  <li><b>Done!</b></li>
</ul>
<h2>Kiero</h2>
<p>You can find Kiero's official repository <a href="https://github.com/Rebzzel/kiero">here</a>
