# Autologin Guest WLAN

[TOC]

## Captive Portal Login

This script allows automatic login to the captive portal using selenium browser automation.

## Install

1. download & install firefox
2. download & install gecko driver [github.com/mozilla/geckodriver/releases](https://github.com/mozilla/geckodriver/releases)
3. copy the unziped binary in a useful folder
4. add the gecko driver folder to your $PATH

Hints e.g here: [how-do-i-install-geckodriver](https://stackoverflow.com/questions/41190989/how-do-i-install-geckodriver)

## Required Adjustments

- [ ] Adjust the XPATH in the script to match the login fields
- [ ] Possibly add `Terms of Use` check field
- [ ] Possibly adjust the command for the enter command

## Python Packages

```bash
pip install pyyaml

pip install selenium
```

## Scheduler

````bash
C:\Users\SER\guestLogin>schtasks /create /tn "Guest relogin" /tr "python captive_portal.py" /sc hourly /mo 6 /st 00:05
````
