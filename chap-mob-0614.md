# ドメイン設定から公開へ～第3回モブ~
6月14日もモブワークの日としました。

この日のワークは、先日取得した独自ドメインを紐づけること、OGPの設定をすること、正式名称を決めていよいよ建設中ページを公開すること、ついでに公式Twitterを取得する、といった内容になります。

![第3回モブ　6月14日のやることリスト]](chap-mob-0614/0614ToDo.png?scale=1.0)


## 独自ドメインをつける

まずは、カスタムドメインで使うための設定です。

順不同で構わないと思いますが、以下の設定を行います。

* DNSレコードの設定

ドメインを取得した会社(レジストラ、販売業者)のドメイン管理画面から、GitHubのIPを指定します。
4つあるので、全て設定しましょう。コンテンツにIPアドレスを入力します。ホスト名の前、wwwなどを入れると想定されますが、使わない場合は空欄でOKです。

https://help.github.com/en/github/working-with-github-pages/managing-a-custom-domain-for-your-github-pages-site
にIPアドレス、他の設定が書いてあります。

![DNSレコードの設定](chap-mob-0614/dnsrecord.png?scale=0.8)

* CNAMEを設定する
/docs/CNAMEというファイルを作り、ここにgipotal.jpと記入します。
/docsがドキュメントルートになる場合ですから、リポジトリのトップがルートになる場合は、当該リポジトリのトップに置くことになります。

* GitHub PagesのSettingsにドメインを記入する。ついでにHTTPSの設定も

先ほど設定した公開Sourceの下にドメインを入れるところがありますので、ここにドメインを入れます。

ついでに、HTTPS強制の設定もあるので、チェックを入れます。

![GitHub Pagesへの設定とHTTPS化](chap-mob-0614/settingfinish.png?scale=0.8)

上に、設定したドメインで公開されたよ！というメッセージが表示されていること、そこをクリックしたらちゃんと表示されることを確認して、設定は終了です。

## OGPの設定をする
OGPとは、Open Graph Protcolの略で、TwitterなどSNSでシェアした時に表示する、サムネイルやタイトル、サイト概要などを定めるHTML要素です。

![OGPの動作例(Slack)](chap-mob-0614/OGPexample.png?scale=1.0)

OGPの記載例を参考に、コードを入れます。

```html
  <meta property="og:url" content="https://gipotal.jp" />
  <meta property="og:type" content="website" />
  <meta property="og:title" content="TOPページ" />
  <meta property="og:description" content="ぎぽたる｜技術同人誌ポータルは、世の中のさまざまな技術同人誌・サークルへの入り口です。ジャンル、イベント横断検索、新着告知など、新しい本との出会いの場です。" />
  <meta property="og:site_name" content="gipotal | 技術同人誌ポータル" />
  <meta property="og:image" content="https://gipotal.jp/images/ogp.png" />
```

OGP用の画像を準備するとか、サイト説明(description) を考えておく必要はあります。

ここでいろいろネタに走ったりするのもモブのご愛敬。
これを作ったら、Slackとかに貼って動作確認をします。

Twitterカードとファビコンの設定もついでにやっておきます。

```html
  <meta name="twitter:card" content="summary" />
  <meta name="twitter:site" content="@gipotal" />
  <link rel="icon" href="https://gipotal.jp/images/favicon.png" sizes="32x32" />
```

### 正式名称を決める
さて、ここまで何となくギポタル/ぎぽたるできてましたが、正式に決めたわけではありません。Twitterのハッシュタグなどにも関わりますので、ここで確定することにします。

muralの投票機能を使って、参加者で投票します。カタカナギポタルが強かったので、それに決定です。同時にロゴ、アイコンのプロトタイプも作ってくれました。

![正式名称の決定](chap-mob-0614/namevoting.png?scale=1.0)


これを以て正式名称がギポタルになり、TwitterIDも@gipotalを取得しましたので、反映します。Twitterの運用は追々です。