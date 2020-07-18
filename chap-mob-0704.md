# フロントを作る(モブ#7 7/4)

モブワークも7回目を数え、いよいよフロントを作り始めます。

## JS環境を設定する
まずは、JS環境の構築です。いつも通り、Zoomでの画面共有と、VSCodeのLive Shareではじめます。
Shared TerminalでRead/Writeの権限を付けることで、ターミナルも使えるようになります。

前回環境をきれいにした状態で、node.jsを入れました。様々な事情によりnvmでのNode.jsデフォルトバージョンが8になっていたので、これを新しいのにします。v12.16.3も別途入っているので、新しい方にしましょう。

```sh
$ nvm use 12.16.3
now using node v12.16.3
$ node -v
v12.16.3
$ pwd
....../tech-book-portal
$ npx create-next-app --template with typescript .
```

Reactだけでなく、Next.jsも使うので `create-next-app` でセットアップをします。

ただ、このとき既存フォルダで作業をしようとして、空のフォルダでないと、コンフリクトする可能性があるというエラーが出ました。

```sh
$ npx create-next-app --template with typescript gipotal
$ mv gipotal/package.json tech-book-portal
$ mv gipotal/public tech-book-portal
$ mv gipotal/pages tech-book-portal
```

そのため既存フォルダから抜け出して、一度新しいフォルダを作り、そこにインストールします。そのうえで `packages.json`と`/pages`と`/public`を対象フォルダ(ここではtech-book-portal)にコピーしました。

```sh
$ cd tech-book-portal
$ npm i
$ npm rum dev
```

これで、localhots:3000にアクセスすると `create-next-app` が作成したデフォルト画面で、動いていることが確認できます。このとき表示されている画面は`/pages/index.js`であり、動作確認のために雑に変更して反映されることを確認します。

初期の`/pages`以下にあれこれ置くのはごちゃっとする問題があります。Next.js 9.1以後は、`/src` というモダンJavaScript界隈で当たり前となっているディレクトリ構造に対応してくれたので、`/src/pages` に移動してしまいましょう。

CTRL+CでNext.jsを止めて作業をします。

```sh
$ mkdir src
$ mv pages src
$ npm run dev
```

表示されることを確認したら、index.jsを適当に整理するとよいでしょう。

![初期のindex.js](chap-mob-0704/indexjs.png)

この11行目のmainタグが、実際にいま表示している内容なのでここらへんを修正していきますが、そもそも他のあれこれも不要なので、大半をごそっと消してしまいましょう。

```jsx {filename=index.js}
import Head from "next/head";
import styles from "./index.module.css";

export default function Home() {
  return (
    <div className="container">
      <Head>
        <title>Create Next App</title>
        <link rel="icon" href="/favicon.ico" />
      </Head>
      <main>ギポっ</main>
    </div>
  );
}
```

こうなるとCSS定義も消えたので、CSSを当てます。

Reactでよく使われるCSSライブラリはemotionなどが有名ですが、色々インストールしないといけなくて面倒なので、Next.js標準で使える CSS Modules を使ってみましょう。

まずは `index.module.css` というファイルにCSSを定義します。

```css {filename=index.module.css}
.hello {
  color: red;
}
```

そして、JS側で`import`します。

```jsx {filename=index.js}
import Head from "next/head";
import styles from "./index.module.css";

export default function Home() {
  return (
    <div className="container">
      <Head>
        <title>Create Next App</title>
        <link rel="icon" href="/favicon.ico" />
      </Head>
      <main className={styles.hello}>ギポっ</main>
    </div>
  );
}
```

これで、文字が赤くなったことがわかり、CSSを当てられるということを確認できます。

ここで、モジュールとして読み込むところから脱線して、いろいろ話が展開しますが、ここでは省略します。いろいろやり方はあるのですが、シンプルなので、CSS Modulesで行きます。

なお、.nextの下にいろいろできてしまうので、.gitignoreに.nextの下を無視するように設定します。

ここでコミットします。なんだかんだで1時間経過してしまいましたが、ここでなべくらさんのマシンにJS環境を作るというところまで完了したことになります。

[32ed0ed8c5e8cb62cfc39abaef5c773f75c1b3cc](https://github.com/onestop-techbook/tech-book-portal/tree/32ed0ed8c5e8cb62cfc39abaef5c773f75c1b3cc)

