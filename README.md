If you want to get SiteMap or DirMap you can use this app.

Use this flags:

        --dir          : creates a dirmap
        --site         : creates a sitemap
        -t             : target dir or site (</home/user> or <https://www.google.com/>)
        -f             : file output
        --duplicate    : if you use this flag then duplicates of sites will be recorded.
        --version      : version app
        -h             : help


Exemple for dirmap:
$ ./SiteDirMap --dir -t ~/TestFolder_start -f ~/fileOutputDir.txt
$ cat ~/fileOutputDir.txt
TestFolder_start
---TestFolder_1
------TestFolder_a
------TestFolder_b
------TestFolder_c
------TestFolder_d
---TestFolder_2
------TestFolder_a
------TestFolder_b
------TestFolder_c
------TestFolder_d
---TestFolder_3
------TestFolder_a
------TestFolder_b
------TestFolder_c
------TestFolder_d


Exemple for sitemap:
$ ./SiteDirMap --site -t 'https://www.google.com/' -f ~/fileOutputSite.txt
$ cat ~/fileOutputSite.txt
https://www.google.com/
---https://www.google.com.ua/webhp?tab=ww
---https://www.google.com.ua/imghp?hl=ru&tab=wi
---https://maps.google.com.ua/maps?hl=ru&tab=wl
---https://play.google.com/?hl=ru&tab=w8
---https://www.youtube.com/?gl=UA&tab=w1
---https://news.google.com/?tab=wn
---https://mail.google.com/mail/?tab=wm
---https://drive.google.com/?tab=wo
---https://www.google.com.ua/intl/ru/about/products?tab=wh
---https://translate.google.com.ua/?hl=ru&tab=wT
---https://www.blogger.com/?tab=wj
---https://photos.google.com/?tab=wq&pageId=none
---https://docs.google.com/document/?usp=docs_alc
---https://accounts.google.com/ServiceLogin?hl=ru&passive=true&continue=https://www.google.com/&ec=%0000000
---http://www.google.com.ua/history/optout?hl=ru
---http://www.google.com.ua/intl/ru/ads/
---http://www.google.com.ua/intl/ru/services/
---https://www.google.com/calendar?tab=wc
------https://calendar.google.com/calendar?tab=wc
---https://www.google.com/preferences?hl=ru
------https://accounts.google.com/ServiceLogin?hl=ru&passive=true&continue=https://www.google.com/preferences%3Fhl%3Dru&ec=%0000000
------https://accounts.google.com/ServiceLogin?service=safesearchlock&ltmpl=lk&hl=ru&continue=https://www.google.com/preferences/sslock%3Fhl%3Dru%26op%3D0%26prev%3D/preferences%253Fhl%253Dru%2526prev%253D/%26sig%3DAAm6Z9sAAAAAX0IdY4KZfDK4hopWU0zXeD4cfMOaFu3Z
------https://accounts.google.com/ServiceLogin?continue=https://www.google.com/preferences%3Fhl%3Dru&hl=ru
---https://www.google.com/advanced_search?hl=ru-UA&authuser=0
------https://accounts.google.com/ServiceLogin?hl=ru&passive=true&continue=https://www.google.com/advanced_search%3Fhl%3Dru-UA%26authuser%3D0&ec=%0000000
------https://support.google.com/websearch?p=adv_safesearch&hl=ru
------https://support.google.com/websearch?p=ws_images_usagerights&hl=ru
------https://support.google.com/websearch?p=adv_pages_similar&hl=ru
------https://support.google.com/websearch?p=adv_pages_visited&hl=ru
------https://support.google.com/websearch?p=adv_operators&hl=ru
------https://www.google.com/preferences?hl=ru-UA
---------https://accounts.google.com/ServiceLogin?hl=ru&passive=true&continue=https://www.google.com/preferences%3Fhl%3Dru-UA&ec=%0000000
---------https://www.google.com#
---------https://www.google.com#languages
---------https://support.google.com/websearch?p=search_help&hl=ru-UA
---------https://support.google.com/websearch?p=ws_settings_safesearch&hl=ru-UA
---------https://accounts.google.com/ServiceLogin?service=safesearchlock&ltmpl=lk&hl=ru&continue=https://www.google.com/preferences/sslock%3Fhl%3Dru-UA%26op%3D0%26prev%3D/preferences%253Fhl%253Dru-UA%2526prev%253D/%26sig%3DAAm6Z9sAAAAAX0IdYieQ39QcYA-6dKHFRh3mEAo3HVkz
---------https://accounts.google.com/ServiceLogin?continue=https://www.google.com/preferences%3Fhl%3Dru-UA&hl=ru
---------https://support.google.com/websearch?p=ws_settings_signin&hl=ru-UA
---------https://www.google.com/history/optout?hl=ru-UA
------------https://accounts.google.com/Login
------------https://www.google.com/policies/privacy/
------------https://www.google.com/policies/terms/
------https://www.google.com/url?q=https://support.google.com/websearch/%3Fp%3Dws_results_help%26hl%3Dru-UA%26fg%3D1&sa=U&ved=0ahUKEwjHtZbM4rDrAhWMz4UKHSx9AZsQ8KwCCAE&usg=AOvVaw3yOJnQxRFDkYC62-Cy9nG4
---------https://support.google.com/websearch/?p=ws_results_help&hl=ru-UA&fg=1
------https://www.google.com/url?q=https://policies.google.com/privacy%3Fhl%3Dru-UA%26fg%3D1&sa=U&ved=0ahUKEwjHtZbM4rDrAhWMz4UKHSx9AZsQ8awCCAI&usg=AOvVaw2Knb9iHSd5qMzB6NEejddj
---------https://policies.google.com/privacy?hl=ru-UA&fg=1
------https://www.google.com/url?q=https://policies.google.com/terms%3Fhl%3Dru-UA%26fg%3D1&sa=U&ved=0ahUKEwjHtZbM4rDrAhWMz4UKHSx9AZsQ8qwCCAM&usg=AOvVaw3viV-K7wnryQGV-1ygRism
---------https://policies.google.com/terms?hl=ru-UA&fg=1
---https://www.google.com/setprefs?sig=0_mWT2OEYXakeC7yAdGFCbKAMHT8g%3D&hl=uk&source=homepage&sa=X&ved=0ahUKEwiF3sPL4rDrAhVIQBoKHcW5AyMQ2ZgBCAU
------https://www.google.com/sorry/index?continue=https://www.google.com/setprefs%3Fsig%3D0_mWT2OEYXakeC7yAdGFCbKAMHT8g%253D%26hl%3Duk%26source%3Dhomepage%26sa%3DX%26ved%3D0ahUKEwiF3sPL4rDrAhVIQBoKHcW5AyMQ2ZgBCAU&hl=uk&q=EgS8o0n4GNisiPoFIhkA8aeDS7R8lfqgBkLN2NL14x2ROKDAWt-WMgFy
---https://www.google.com/intl/ru/about.html
------https://www.google.com/intl/ru/about/
---------https://about.google/intl/ru/
---https://www.google.com/setprefdomain?prefdom=UA&prev=https://www.google.com.ua/&sig=K_9wHZcCR_Nmsz5Ovbw6nhoeMfpfU%3D
------https://www.google.com.ua/
---https://www.google.com/intl/ru/policies/privacy/
------https://policies.google.com/privacy?hl=ru
---https://www.google.com/intl/ru/policies/terms/
------https://policies.google.com/terms?hl=ru
