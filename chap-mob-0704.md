# フロントを作る(モブ#7 7/2)

モブワークも7回目を数え、いよいよフロントを作り始めます。

# JS環境を設定する
まずは、JS環境の構築です。いつも通り、Zoomでの画面共有と、VSCodeのLive Shareではじめます。
Shared TerminalでRead/Writeの権限を付けることで、ターミナルも使えるようになります。

前回環境をきれいにした状態で、node.jsを入れました。デフォルトがバージョン8になっていたので、これを新しいのにします。v12.16.3も別途入っているので、新しい方にしましょう。

```sh
$ nvm use 12.16.3
now using node v12.16.3
$ node -v
v12.16.3
$ npx create-next-app --template with typescript .
```
Reactだけでなく、Next.jsも使うので、入れる方針で行きます。

空のフォルダでないと、コンフリクトする可能性があるというエラーが出ます。ですから、一つ上の階層に上がり、新しいフォルダを作り、そこにインストールします。そのうえで、package.json /pages /publicを対象フォルダ(ここでは/tech-book-portal)にコピーします。

```sh
$ nvm install
$ npm rum dev
```

これで、localhots:3000にアクセスすると、動いていることが確認できます。

/pages/index.jsが表示されているファイルですから、適当に変更して反映されることを確認します。

ルートはごちゃごちゃしがちなので、/src/pages/に移動しちゃいましょう。Ctrl+Cで一度止めて、npm run devで再起動してやると、反映されます。

表示されることを確認したら、index.jsを適当に整理するとよいでしょう。

Mainの中が現在表示しているものなので、Main の中を消してしまえば

StyleやFooterの中身も不要なので消してしまいます。Styleの直書きなどの話も出てくるのですが、一旦置いておきます。Style消すと、Styleが当たらなくなるので、中央寄せから、左上に寄ることになります。

cssを外出しするには、index.cssを作り、これをインポートします。

```js
import styles from './index.module.css'

    <main className={styles.hello}>
    ギポっ
    </main>
```

index.module.css

```css　
.hello {
  color: red;
}
```

これで、文字が赤くなるはずです。普通のcssはjsで読めないのですが、module.cssと書いてモジュールとしてインポートすることができます。