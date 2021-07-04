#!/usr/bin/env python3

import os
import yaml
import time 

from selenium import webdriver
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support.select import Select


if __name__ == "__main__":

    debug_sleep = 10

    # load config from file
    with open(os.path.join(os.path.dirname(__file__), "config.yaml")) as file:
        config = yaml.load(file, Loader=yaml.FullLoader)

    # get browser control instance
    browser = webdriver.Firefox(webdriver.FirefoxProfile())

    # go to site
    login_url = "https://www.swisspass.ch/oevlogin/login"
    browser.get(login_url)

    # find user and password field in DOM
    user = browser.find_element_by_name("USERNAME")
    password = browser.find_element_by_name("PASSWORD")

    # clear the input fields
    user.clear()
    password.clear()

    # add user/password from config
    user.send_keys(config.get("username"))
    password.send_keys(config.get("password"))

    # click login button
    login_btn = browser.find_element_by_name("LOGIN")
    login_btn.click()

    time.sleep(debug_sleep)

    # go to site
    logout_url = "https://www.sbb.ch/public/logout"
    browser.get(logout_url)

    time.sleep(debug_sleep)

    # browser cleanup and close
    browser.delete_all_cookies()
    browser.close()
