# 画面を作っていく(モブ#7 7/9)

前回までで、バックエンドのHasuraを、フロントエンドのReactからアクセスし、クエリによりデータを引き出すことができました。あとはもうプロジェクトに合わせて作り込んでいくだけです。いよいよページの本体を作り始めましょう。まずはサークルページからはじめます。

## サークルページを作る
トップ画面を作る？サークル画面を作る？本の画面を作る？そのうえで、データはべた書きで、CSSをいくつか書いていく形で進めるのがよさげ、ということで、サークル画面を作ることにしました。

![Figmaで組んだサークルページ](chap-mob-0709/circlefigma.png?scale=0.8)

実際に始める前に、CSSのタグに絡むWarningが出た話から、CSS modulesとグローバルCSSの違いの話になります。

HTML全体、たとえばbodyとかにCSSを当てたい場合は、グローバルなCSS[^global-css]を Next.js のグローバルに読み込めるページファイルである `pages/_app.js` から読み込みます。

CSS Modules は元々コンポーネント単位に閉じた CSS [^local-css]を実現するためのものです。

[^global-css]: https://nextjs.org/docs/basic-features/built-in-css-support#adding-a-global-stylesheet
[^local-css]: https://nextjs.org/docs/basic-features/built-in-css-support#adding-component-level-css

そういった解説が始まってから、モブワーク開始です。Figmaで仮組したサークルページを見ながら進めます。

Figmaで組まれたデザインを実際のHTML+CSSに落とし込むためには細かい情報が必要になります。このとき、Alt(MacならOption)を押しながら要素を触ると、要素間のマージン(要素間の距離)が見えたりします。ですので、これに合うようにCSSを当てていくのが基本的な動きになります。

さて、Reactでフロントエンドを開発していく工程としていったんデザインを後回しにして、「箱（コンポーネント）から作っていく」という方法で進めます。サークルページは、ヘッダーや、サークル説明の詳細などに分割できます。

ここまでの作業では `.js` というファイルを触ってきましたが、今の時代 TypeScript を使うのが定番なため `pages/circles/[id]/index.tsx` を作り、これをサークルページとします。

#### [column] TypeScriptの設定でパスエイリアスを設定する

参照関係が複雑になってくると、`../../componenst/hoge` のように階層が複雑になることがあります。そこで、tsconfig.jsonにpathを設定してシンプルに読み替えることができます。

```js
  "compilerOptions": {
    "baseUrl": ".",
    "paths": {
      "@/*": ["src/*"],
    },
```
`@/`と書くだけで`src/`を参照するようにできます。

#### [/column]

### Headerを作ろうとして、インポートの話で盛り上がる

ヘッダーを作ります。が、その前に、ES Modulesによるインポートの話[^import]をひとしきりやります。結果としては予想外に盛り上がりました。

JavaScript/TypeScriptのコードでは、コードの先頭で使用するモジュールをインポートします。インポートするものは、モジュールでエクスポートしている何かです。

このインポートには何種類かのやり方があるため、その解説話になりました。

```js
import React from "react"; // React というでかいすべてを取ってくる
import { useRouter } from "next/router"; // useRouter という単体の関数を取ってくる
import { NextPage } from "next"; // NextPage という単体の型を取ってくる
```

ここでは2つの方法によるインポートが書かれています。`import React from "react";` はコメントにも書いてあるように react というパッケージで公開されている全て[^default-export]を `React` というでかい単位で取得します。実際に使う時には、`React.FC` や `React.useState` のような感じで、`React.` という感じでアクセスをします。

[^default-export]: もう少し踏み込んでいうと、これはデフォルトインポートというべきもので、元のモジュールで `export default` されているものをインポートするものですが、さらにややこしいことにより、TypeScriptの機能により `export default` されているものがない場合には、全部をひっくるめたものとしてインポートすることができます。

もう1つのインポートは2行目と3行目のような波カッコがついた表記で、名前付きインポートです。これは全部、あるいはデフォルトをとってくる、みたいな挙動ではなくて、名前付きでエクスポートされているものを、狙ってインポートするのです。

`useRouter` は `next/route` というパッケージ（にディレクトリをつけたもの）からインポートし、`NextPage` は、`next` パッケージから、それぞれ対象のものだけをインポートしています。

波かっこが付く場合と付かない場合について、あるいは分割代入について説明、共有がなされ、何となく共通認識ができたような気がします。

```js
const { id } = router.query // 分割代入
const id = router.query.id
```
この2行は同じ内容を意味します。

`NextPage` 型は React.FC に、Nextのページ型固有の情報（`getStaticProps` や `getServerSideProps` が付与されたもの）です。

こうやって NextPageという型をインポートしたことで、補完が効くようになったり、型支援が効くようになり、いろいろ便利になります。関数として定義されるので、想定しない引数が指定されたりしたときにエラーが出るようになります。

[^import]: https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Statements/import


このあたりの準備と説明で思ったより時間がかかってしまいましたが、いよいよ実際に本体を作り始めます。まずはヘッダーから。

### ヘッダータグをつける

まずは、画面上のヘッダー部位にサークル名を表示します。親方Projectと表示しましょう。
```js
const Header: React.FC = () => {
  return <header>親方Project</header>
}

const CirclesShow: NextPage = () => {
　return <Header />
}

export default CirclesShow
```

これで、ブラウザ上には、親方Projectという文字が表示されるはずです。

同一のファイルに複数のコンポーネントを書くのは好ましくないケースもありますが、まずはべた書きして、必要になったときにファイル分割を行いましょう。

### 画像を画像を貼り付ける。

次は、ロゴ画像を貼り付けます。ヘッダーのトップに置きます。

ファイル操作するときに複数人が良かれと思って操作をすると混乱をしてしまうので、ホストの人がファイルを置く、みたいに決めておくと混乱、お見合いといった状況を避けられるでしょう。さらにいうと、LiveShareの場合、生ファイルを置こうとするとうまくいかないケースもあるようなので、ホストの人が操作するのが無難なようです。

画像として参照する場合、画像ファイルは、`public/images/logo.svg` に置きます。`<img src="/images/logo.svg"/>`

#### [column] オートフォーマット

Macなら Shift+Option+F で、機械が勝手にインデントを修正するなど、オートフォーマットをしてくれます。見た目が分かりやすくなりますし、人間が手動でインデントをするのが好ましくないため、オートフォーマットを活用するといいでしょう。

ただ、VSCode LiveShare ではリモートからフォーマットできないといった問題があるので、ホストの人が定期的にフォーマットのキーを叩くとよいでしょう。

### ボタンを作る

ヘッダには、ユーザーメニューとして「新規登録」「ログイン」というボタンを配置します。

まずは`<Link href="/signup">新規登録</Link>`のようなテキストで書きます。

次に、これをボタンに見た目に変えます。

`index.tsx`と同じところに `index.module.css` というファイルを作り、`index.tsx` から読み込みます。

```css
.blackButton {
  font-family: Roboto;
  font-style: normal;
  font-weight: bold;
  font-size: 14px;
  line-height: 16px;
  padding: 13px 24px;
  border-radius: 50px;
  color: #fff;
  background-color: #000;
}
```

フォントなどの指定、paddingや、角を丸くするための `border-raduis` を指定し、色を指定しています。

```tsx
import styles from "./index.module.css";

  <Link href="/signup">
    <a className={styles.blackButton}>
    新規登録
    </a>
  </Link>
```

ただ、この書き方では `whiteButton` にも同じようなコードが並ぶ羽目になるため `.button` と `.blackButton` `.whiteButton` を切り分けるようにリファクタリングします。リファクタリングの基本は「前後で挙動が変わらない」ですので、つど見た目を確認しながら進めます。

```css
.button {
  font-family: Roboto;
  font-style: normal;
  font-weight: bold;
  font-size: 14px;
  line-height: 16px;
  padding: 13px 24px;
  border-radius: 50px;
}

.blackButton {
  color: #fff;
  background-color: #000;
}

.whiteButton {
  color: #000;
  background-color: #fff;
  box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.12);
}

/* ボタンとボタンの間を開ける */
.button + .button {
  margin-left: 10px;
}
```

```js
import styles from "./index.module.css";
...
        <Link href="/signup">
          <a className={[styles.blackButton, styles.button].join(" ")}>
            新規登録
          </a>
        </Link>
        <Link href="/login">
          <a className={[styles.whiteButton, styles.button].join(" ")}>
            ログイン
          </a>
        </Link>
...
```

2つ以上のCSSクラスをあてたい場合、文字列結合する必要があります。そのために、配列の `.join` メソッドに引数 " " を渡すことで結合しています。

Shift+opt+Fでオートフォーマットをかけて、本日はそろそろおしまいですね。開始から2時間経過しましたので、振り返りを軽くやっておしまいです。

#### [column] Web会議で目線が合わない

雑談かつ、リモート会議あるあるです。そもそも目線を合わせる必要がないのはありますが、PCのカメラではうまく合わないときの話。

大きめディスプレーの上にWebカメラを置いて、映っているものをディスプレーの上方に置くと、目線を合わせやすくなります。そもそも、合わせる必要があるのか、もっと言えば、顔を映す必要があるのか、というあたりに疑問がありますが。

#### [/column]

### 厳密には違うんだけど、という説明

この日のモブの中で、importとは、あるいはほかの「似たもの」の違いについての説明がたくさんありました。説明を丁寧にしていた結果、表向きの今日の進捗はロゴを貼ってボタンを2個作っただけです。しかしそれ以上に学びの多かったモブだと感じでいます。

`import React from 'react'` と `import * as React from 'react'`にも差があります。一方で、同じimportです。説明者にとっては、厳密な差を把握した上で、みんなにわかるように説明しなければならないという非常に大変なワークです。脳がプスプスするというかわいい表現が出てきました。

世の中の多数の書籍やブログでは、「とりあえずこう書いときゃいいんだよ」　「おまじないです」とロクに説明していないことも少なくありません。もちろんいちいち説明するのも大変なのもわかります。分量的にも、読者の理解的にも大変です。ただし、Tutorialレベルではおまじないはアリですが、実際のProductのコードに対してそれは厳しいですよね。でもそれを理解するすべがあるかというと微妙なところです。

動いているからいいじゃん、何故そう書いているか理解しない、という場合、たいていは少し変更したら動かなくなるとか、技術的負債が蓄積するのです。

https://ja.reactjs.org/docs/getting-started.html　React-Getting started

React だったら、Getting startedを全部読むのが望ましい。というコメントもありましたが、それはそれで大変です。それができたら苦労しないっす。とはいえ、そこをちゃんと理解しているということ自体、大きな武器になりますね。

## まとめと振り返り
サークルのトップページを作り始めました。ロゴ画像を表示したり、Headerを作ってヘッダーコンポーネントを配置したり、ボタンを作って、形を整えたり新規登録やログインという表示をしたり。また、Figmaほんとに便利ですね・・・イチから作るのと比べるとダンチ・・・素敵なツールです。

それとともに、importの違い(デフォルトインポートと単なるインポートの違い)などを共有しました。次回予定は、7月11日に、情報を載せていくのをやる予定です。

![本日やったこと](chap-mob-0709/0709-done.png?scale=0.8)

![7月9日のFun Done Learn](chap-mob-0709/0709fundonelearn.png?scale=0.8)