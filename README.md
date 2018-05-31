# CPL Dropper
A Control Panel Applet dropper project. It has a high success rate on engagements since nobody cares about .CPL files and you can just double click them. You'll need to modify the base64 string array within Data.h to include your payload and the OnDblclk function in ControlPanelApplet.cpp to modify your output files. Essentially, the application writes your base64 encoded array to file and then uses certutil to base64 decode it. It uses cmd.exe to run it thereafter. I take no responsibility for how this is used, so be nice :)

## Red Team Goals
Control Panel Applets (.cpl) are one of those older Windows file extensions which exist but vanish from the limelight as latest and greatest techniques come and go, yet it remains. When you open your Control Panel in Windows, those are in fact DLL files called Control Panel Applets. The difference is that you can have functions which trigger upon doubleclick, unlike a DLL's entry points. Most of the time I've found that mail servers don't care about CPL files and Anti-Virus doesn't care about it either.

Generate your .exe payload and convert it into a base64 encoded array. I have a builder I'm working on which will automate this, but for now here's a bash one-liner to help:
```
cat payload.exe | base64 | sed -e 's/\(.\)/\1,/g' | sed 's/\,/\x27\,\x27/g' | rev |  cut -c 3- | rev | sed -e "s/^/{\x27/g" | sed -e '$s/$/}/' > base64_encoded_array.txt
```
Add this array as the value for base64[] within Data.h. Modify base64path and targetpath variables within the OnDBLclick function in ControlPanelApplet.cpp to which ever path and filename you like. What I've noticed is that Anti-Virus is very focussed on %localappdata% but if you hide your stuff further inside other folders it will be ignored. 
Compile and change the extension to .cpl. Your payload is now ready for your red team campaign.
## Blue Team Goals
When I first started playing with this extension a few years ago, the first thing that I noticed is that most mail server don't have any rules to process .cpl attachments. Even GMail allowed cpl files through for a while, until recently (crazy right?). The very first thing that I did was frantically run to our IT guy and say "Dude you have to block this extension now!"

You should make sure that these file extensions cannot get through your mail server. Moreover, increase the difficulty of execution by using AppLocker rules which only allow signed binaries. Before red teamers flame me for this comment, yes I know it's not too hard to sign your payloads to get past this...but defense in layers right? :) 

For more information, check out Oddvar's cool article on this (big fan): https://oddvar.moe/2017/12/13/harden-windows-with-applocker-based-on-case-study-part-1/

Please contribute to this one and help me make this more awesome. Thanks friends.
