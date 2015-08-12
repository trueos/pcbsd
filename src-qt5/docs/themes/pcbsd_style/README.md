#PC-BSD Style 

###A new style for _Our_ docs so as to avoid upstream theme changes.

**In order to use the pcbsd_style theme, there are some adjustments to make to the _src-qt/docs/conf.py_ file:**

* Revise this value from 'classic' or something else, to 'pcbsd_style' prior to generating docs:
```
  html_theme = 'pcbsd_style'
```
* Comment out these lines because the build may fail since these don't exist and/or aren't used right now:
```
   #    "stickysidebar": "true",
   #    "rightsidebar": "false",
   #    "sidebarwidth" : "240",
   #    "headbgcolor" : "#fff",
   #    "relbarbgcolor" : "#696969",
   #    "sidebarbgcolor" : "#696969",
   #    "bgcolor" : "#fff"
```
* Add or adjust this statement below to match, it is for _additional_ places to search:
```
  html_theme_path = ['themes']
```
