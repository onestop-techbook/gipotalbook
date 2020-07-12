# サークルページを組む（モブ#9 7/11)
いよいよサークルページを組み始めます。

その前に、サークルページに表示する内容を再確認しましょう。

## サークルページに表示する内容
先日のDBに格納する内容は概ね共有しました。しかしサークルページと考えた場合、改めて必要なもの、不要なものをデザインとして考える必要があります。

まずは、技術書典のサークルページ、頒布物ページから考えます。

ふりがな、メールアドレスなどはユーザー情報として必要ですが、サークルページに表示する必要がない情報です。まずはこれらを整理します。

トップに表示される内容としては、サークル名、サークル紹介文、参加イベントリスト、頒布物の一覧、そして、Webサイトとして、サークルごとのWebサイト、GitHubやTwitter、Boothのトップページなどが挙げられます。また、サークルのメインになるジャンルなども表示して欲しいところですね。

必要な項目、必要だけど優先度の低い項目を整理したら、モックアップと比較しつつ、デザインとして追加していきます。

GitHub、Twitter、Boothなどは、リンクとしてグループ化して考えます。

他のイベントのページとして、技書博、コミケ（Circle.ms)もチェックします。同じような目的ですから、内容は似たようなものになりますね。

イベントのページにはあって、このポータルでは不要な項目としては、配置があります。あくまでイベント横断のポータルサイトですから、サークルトップには記載しません。

7月２日にDB構築の中で確認・抽出した項目と比較し、漏れがないか確認します。商業誌声かけフラグが漏れていたので、これを抽出し（ただし、実装は後ほど）となります。MVPとしてこれが必要かという判定を行うと、優先度が下がるためです。

また、寄稿者・著者一覧は、本に紐づく情報なので、本のページになります。

## CSSリセットをする

ブラウザはデフォルトのCSSを持っているのですが、今時デザインするに邪魔なのです。ですから、最初にリセットして、自前のCSSを適用するように行うものです。

_app.js というファイルを作り、ここに、CSS Remedyをコピペして貼り付けます。

https://coliss.com/articles/build-websites/operation/css/css-reset-for-modern-browser.html

貼り付けた直後は一行のままですが、オートフォーマットを使います。
Shift+Alt+Fで改行を入れたフォーマットに変更することができます。
全ページに適用されます。

例えば、リンクの下線を引かないようにする場合、
a {
    text-decoration: none;
}
と付け加えます。すると、全ページのリンクから下線が無くなります。

## トップページを作る

では、先日作ったヘッダーページに項目を順次付け加えていきます。

まずは、背景を貼り付けます。もっとさんがまだ来てなかったので、とりあえずFigmaから画像を取ってきて貼り付けます。FigmaのExportで画像を保存します。

CSS