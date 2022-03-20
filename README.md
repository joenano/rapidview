# rapidview
GUI JSON viewer

For most cases, web based viewers are fine, but when working with large JSON files they become unresponsive or unusable and native performance is needed. I couldnt find a desktop app that I liked so I made this one. Its just a basic GUI view with [rapidjson](https://rapidjson.org/) backend.

I may add editing capabilities in future if there is any interest.

![GUI](https://i.postimg.cc/SNgzN01g/gui.png)

### Settings
The only setting currently is for colour coded types, this is false by default as it has a cost of about 40% speed on opening JSON files. Can be set to true in settings/settings.json file.

