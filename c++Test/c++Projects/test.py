def get_topics(url):
    filename = str(re.findall(r'(?<=custom:).+?(?=&Refer)', url)[0])
    res = requests.get(url, headers=head)
    html = etree.HTML(res.text)
    keys = ['name', 'user_url', 'text', 'translation_num',
            'comment_num', 'like_num', 'time']
    m = ['', '', '', '', '', '', '']
    blank = dict(zip(keys, m))
    blank = pd.DataFrame(blank, index=[0])
    try:
        pages = len(html.xpath('//ul[@class="s-scroll"]/li'))
        print(pages)
    except:
        pages = 1
        for page in range(1, pages+1):
            final_url = url + '&page={}'.format(page)
    if 'carry out the full text ' in text:
        unfold_url = html.xpath(
        '//div[@class="cardwrap"][{}]//a[@action-type="fl_unfold"]/@href'.format(n))
        #print('https:' + unfold_url[count]+'&type=comment')
    try:
        txt = requests.get(
            'https:' + unfold_url[count]+'&type=comment', headers=headers)
        txt.encoding = 'utf-8'
        try:
            text = re.findall(r'(?<=nick-name=).+?(?=<a)', txt.text)[0]
            text = str(text).replace('&nbsp;', '').replace(
                name, '').replace('\\n', '').replace('\\"\\">', '').strip()
            if text == '':
                text = re.findall(r'(?<=">#).+?(?=<a )', txt.text)[0]
                text = '#' + str(text).replace('\\', '').replace('/',
                        '').replace('a>', '').replace('<', '')
        except:
            with open('E:/wrong_url.csv', 'a+', encoding='utf-8') as w
            w.write('https:' + unfold_url[count]+'&type=comment'+'\n')
            print(text, '\n', name)
    except:
        print(unfold_url)
        count += 1
        translation_text = html.xpath("//a[contains(text(),')]/text()")[n-1]
        translation_num = re.search(r'\d+', translation_text)
