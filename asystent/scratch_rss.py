import urllib.request
import os

try:
    req = urllib.request.Request("https://cointelegraph.com/rss", headers={'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'})
    with urllib.request.urlopen(req) as response:
        html = response.read()
        with open("rss_test.xml", "wb") as f:
            f.write(html)
    print("Success. Size:", len(html))
except Exception as e:
    print("Error:", e)
