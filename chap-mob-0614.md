# ドメイン設定から公開へ～第3回モブ~

## 独自ドメインをつける

次は、カスタムドメインで使うための設定です。

順不同で構わないと思いますが、以下の設定を行います。

* DNSレコードの設定

ドメインを取得した会社(レジストラ、販売業者)のドメイン管理画面から、githubのIPを指定します。
4つあるので、全て設定しましょう。コンテンツにIPアドレスを入力します。ホスト名の前、wwwなどを入れると想定されますが、使わない場合は空欄でOKです。

https://help.github.com/en/github/working-with-github-pages/managing-a-custom-domain-for-your-github-pages-site
にIPアドレス、他の設定が書いてあります。

![DNSレコードの設定](chap-mob-0614/dnsrecord.png?scale=0.5)

* CNAMEを設定する
/docs/CNAMEというファイルを作り、ここにgipotal.jpと記入します。
/docsがドキュメントルートになる場合ですから、リポジトリのトップがルートになる場合は、当該リポジトリのトップに置くことになります。

* GithubPagesのSettingsにドメインを記入する。ついでにHTTPSの設定も

先ほど設定した公開Sourceの下にドメインを入れるところがありますので、ここにドメインを入れます。

ついでに、HTTPS強制の設定もあるので、チェックを入れます。

![GithubPagesへの設定とHTTPS化](chap-mob-0614/settingfinish.png?scale=0.5)

上に、設定したドメインで公開されたよ！というメッセージが表示されていること、そこをクリックしたらちゃんと表示されることを確認して、設定は終了です。