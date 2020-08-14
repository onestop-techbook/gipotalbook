# Hasuraを構築する（モブ#12 7/30)

連休やなんやらで少しあきましたが、今日はHasuraを構築し、実際にページとして表示します。

みんなで使えるGraphQLサーバーを立ててみましょう。

Hasura.cloudというのはあるのですが、月99ドルからなので現実的ではありません。ローカルで立ててもデプロイの問題が出てくるので、Herokuでやってみます。

規模が難しいとか何か機能として無理になるとかの問題が生じたらその時考えることにしましょう。

## Herokuをみてみる
Herokuでやってみることにします。まずは、料金などの確認です。ギポタルでチームを作ります。５ユーザーまでは無料のチームが作れるようです。ただし、クレカ登録が必要です。一旦かからないけど、チーム人数が超えた時などに請求される形だと考えられます。

とりあえず、いまは個人IDで動作の練習とデプロイを確認してみます。まずは、ID作って、ログインをします。

### GraphQLをデプロイする
HasuraからHerokuにデプロイするチュートリアルがあり、その中にデプロイするボタンがあるので、ここからデプロイします。

https://hasura.io/docs/1.0/graphql/manual/deployment/deployment-guides/heroku.html

とりあえずギポタルに干渉しない名前をつけておいて、Deploy appをクリックすると、デプロイされます。Dockerのログが走り、しばらくすると完了し、Hasuraのコンソールが見れるようになります。

無料プランではHerokuの資源が若干貧弱なため起動には多少時間がかかりますが、Hasuraのコンソールが見れれば完了です。本当にワンクリックでできたよ、とみんなから驚きの声も。

あとは、Hasuraの画面からテストデータを作ります。Add new tableから模擬データを作ります。この辺りは、過去のモブでやったところですね。

ところでここで、データタイプの型にName型というのがあることに気づき、なんじゃこりゃ、とう話も出ます。まあテキストでいいですよね。となります。

idとnameを適当に作って模擬データとします。たくさんありすぎても仕方ないので、二つ三つです。

そして、GraphQLのURLをコピーして、VS codeのindex.tsの中にある、URLを書き換えます。

Next.jsの画面を更新すると、データが読み込まれ、先ほど入力したデータが表示されれば成功です。

万全を期すたえHasuraにデータを追加して、再度更新してみます。追加したデータを含めて表示されることを確認すれば万全です。

### GraphQLのURLを隠す
このGraphQLは表示して良いものでしょうか？不用意なアクセスによるセキュリティリスクもありますから、隠すことも検討します。とりあえず隠匿できるように。.env設定もやってみます。

プロジェクトのルートに.envというファイルを作ります。

```js .env
GRAPHQL_URL = "https://hoge.heroku/hoge"　//変更後1
NEXT_PUBLIC_GRAPHQL_URL = "https://hoge.heroku/hoge"　//変更後2。OK
```

```js index.tsx
  url: https://hoge.heroku/hoge, //もと
  url: process.env.GRAPHQL_URL,　//変更後1、クライアントサイドに公開されるのでNG
  url: process.env.NEXT_PUBLIC_GRAPHQL_URL,　//変更後2、OK
```
としてみます。が、これでは動きません。Nextを再起動したところで。起動ログに.envが読み込まれている記載があったのでその点は問題ありません。

これが動かないNEXT.jsはサーバーサイドにもクライアントサイドにも利用されるのですが、変更後の環境変数がクライアントに公開されてしまうことになります。そこで、環境変数をクライアントサイドに公開するのかしないのかを指定する必要があるのです。

なお、隠匿するものだけでなく、環境ごとに変えたいといったときにも使えます。

HasuraのURLは隠匿するようなものでもないので、このままPushしておきます。今後隠匿が必要なものが生じたらそのときに設定します。

ただし、.envはGitHubの無視するファイルに含まれています。ですからリポジトリで公開する場合に追跡されません。

これでは他に使ってみようと思った人が困ってしまいます。そこで、`.env.exaple`を作り、公開しておくとともにセットアップ方法をReadmeに記載します。初めてリポジトリ

```sh
$ npm install 
$ cp .env.example .env
# ローカルでHasuraを立ち上げる人のために
$ docker-compose up
# ここに一つ足りないものががある
$ npm run dev
```

環境変数を設定するところを作っておきますが、動かすにはまだ一つ手順が足りません。

それは、立ち上げたHasuraにダミーデータを作るところです。また、安全に立ち上げるためにはデータのインポート・エクスポートができる必要があります。

そこで、一旦環境変数を構築、というところでコミットしておきます。

``` .gitignore
# dotenv environment variable files 
.env*
!.env.example
```
として、`.env`は無視するが、`.env.example`は無視から除外し追跡・アップロードできます。

一旦これでおしまいですね。

### リポジトリの名前を変える
Tech-book-portalというリポジトリ名だったのですが、正式名称も決まったことですし、リポジトリ名自体を変えます。

```sh
git config -e
```

これで設定項目が開きますから、`[remote "origin"]`の中に記載されているURLを新しいリポジトリに変更します。viで開いてしまったため、viの使い方講座が始まったのは別の話。

デプロイしてここはおしまいです。ついでに、Next.jsを9.5に更新しておきます。

## HasuraのSchemaをどうやって管理する？
次は、Hasuraの中でデータをどう管理するのか？というところです。

ですから、Hasuraが見ているPostgresSQLのマイグレーションのための調査、確認を行います。

Hasuraの機能として、変更されたものをMigrationを自動生成して取り扱うことができるようです。
基本的には、Herokuの上で操作するわけではないので、Migrationファイルを突っ込むだけでできます。そこでdocker-compose.ymlを書き換えます。

``` docker-compose.yml
#11行目
image: hasura/graphql-engine:v1.2.2  #Before
image: hasura/graphql-engine:latest.cli-migrations-v2 #After
#19行目
HASURA_GRAPHQL_ENABLE_CONSOLE: "true" # Before set to "false" to disable console
HASURA_GRAPHQL_ENABLE_CONSOLE: 'false' # After
```
Docker-composeを再起動しようとしたところ、Dockerがそもそも起動していないようだったので、Dockerをデーモンとして起動します。そして、Hasuraのコマンドラインツールをインストールします。あとは、初期化し、

```
$ docker-compose up -d
$ npm install hasura-cli
$ npx hasura init
# Project directoryを作る
$ ls giotal
config.yml metadata migrations　
$ npm run hasura migrate create init --from-server
INFO Migrations file created
$ npm rum hasura console
# SQLファイルが生成されます。
# Hasura Consoleでデータを追加し、Migrationデータが更新されることを確認して完了。
```

``` package.json
  "Scripts":{
    "hasura": "hasura -- project hasura" #追加
  }
```

ただし、これをそのままやると、消したという履歴が残ってしまうという不整合問題が生じる可能性があります。そろそろきちんとテーブル設計をはじめないといけないですね・・・

「マイグレーションの仕組みを追加」とコミットメッセージを書いてコミットします。フロントの人ばっかりなのに、完全にインフラ・サーバーサイドのことをやってますね。まあそういう日もあってよいでしょう。

Hasura関係でやっておきたいことが二つあり、それをついでにやってしまします。

一つ目は、ApolloClientの初期化を個別ページでやっているので、それをindex.tsxから_app.tsxにお引越しします。

動作確認のために、サークルページにProfilesを表示してみます。

```js [id]index.tsx
import Profiles from "@/components/profiles"

<Profiles />  #たとえばHeader内にProfilesとして、表示データを一括で表示。
```
ApolloClienstが最新版じゃないため動かない？ということで、確認してみると、ちょうどメジャーアップの狭間だったということになります。一度node_modulesとpackage_lock.jsonを消して、再インストールしてみたところ動きました。何もしてないのに壊れ、何もしてないのに直るという状況。これで動いたら笑うんだけど、と軽口をたたきながら、実際に動いてしまいます。いずれにせよ、こういうことができるのがやはりモブのいいところですね。しかもみんなで相談することで、いろいろな可能性や選択肢を見つけたりトライしてみたりすることができるのが素晴らしいところですね。

これで、Hasuraで登録したデータを持ってくることができるようになった、ということになりますので、必要なデータをデータベースから持ってきて表示することができるようになったということです。

もう一つのやりたいことは、コードジェネレーターの追加です。

package.jsonのdependanciesに追加します。

クエリでHasuraからデータを取ってくるときに、せっかく型のある言語なのに型が付きません。そこで、TypeScriptの型情報を吐き出すなどに必要なツール群を追加します。

```js package.json
  "Scripts":{
    "codegen": "gql-gen" 
  }
  "dependencies":{
    "@graphql-codegen/cli": "^1.17.0",
    "@graphql-codegen/introspection": "^1.17.0",
    "@graphql-codegen/typescript": "^1.17.0",
    "@graphql-codegen/typescript-operations": "^1.17.0",
    "@graphql-codegen/typescript-react-apollo": "^1.17.0",
  }
```

codegen.ymlファイルを作り、その中に以下のコードを追加します。

```js codegen.yml
schema: http://localhost:8080/v1/graphql/
documents:
  - ./src/**/*.graphql
overwrite: true
generates:
  ./src/generated/graphql.ts:
    plugins:
      - typescript
      - typescript-operations # .tsx に書いた query, mutation 等から対応する型を生成する
      - typescript-react-apollo # .tsx に書いた query, mutation 等から対応する hooks を生成する
    config:
      skipTypename: false
      withHooks: true
      withHOC: false
      withComponent: false
``` 

それができたら、nmp installで関係パッケージのインストールを進めます。

components>profiles.graphqlというファイルを作り、
もともとのクエリを貼り付け、少し書き換えます。

```js profiles.graphql
query get profiles{
  profiles{
    id
    name
  }
}
```
generatedというフォルダができるので、その中にgraphql.tsというファイルができています。

profile.jsに、
```js profiles.graphql
//import {useQuery} form "@apollo.react-hooks";消す
import useGetProfilesQuery from "@/generated/graphql"
・・・
const {loading,error,data}　= useGetProfileQuery();
・・・
```
と書き換えます。使うクエリは変わりましたが、動作は変わらないことを確認します。また、中の変数を見て(たとえばprofile)を見ると、型情報が付いていることが確認できます。また、クエリ自体を外だしできるというメリットもあります。

.graphqlというファイルにクエリを外だしすることができ、かつそれをアクセスするための関数(useGetProfileQuery)がかってに生成されたことになります。ComponentとQueryの分割ができ、見通し、管理がしやすくなります。利用する側からは、関連するクエリを使えるようになりますい、クエリだけを作っていくことでそれを読み取り配置していくだけにすることも可能になります。

```js sampleQuery
query getCircle($id: Int!) {
  circles(where: { id: { _eq: $id } }) {
    id
    name
    user {
      display_name
      id
      screen_name
    }
    description
  }
}
```
サークル情報を取るクエリは、引数を増やし、上記のように書くことができます。


ここで悩ましいのは、generatedを共有するかどうかです。個人で生成すべきという考え方もできますし、生成したものを共有することで漏れがなくなったりというメリットがあります。diffが大きくなったりコンフリクトが起こったりする可能性があります。外す場合は、gitignoreに入れて置けばよいでしょう。

また、npm run devをウォッチしておき、不用意な生成を監視しておくという選択も可能です。実運用は後日考えます。

今日はこれくらいにしておきましょう。

一旦、graphqlコード生成(/src/generated)は.gitignoreに追加しておき、GitHubpushされないようにします。

#### [column] やったことがわかるとコミットメッセージは簡単
一連のワークの概要がわかったら、コミットメッセージをつけやすいですね

案外何を書くか悩むものですが、その変更で何をやったのかといったところが把握できていると、個のコミットでは何をやったか、というところが書きやすくなります。
#### [/column]

#### [column] 小ネタ:Spotlight検索
Macの上のバーの右端近くに虫眼鏡があります。これをクリックすと、Spotlight検索開きます。アプリを直接探して検索したり、電卓として計算をしたり、便利に使えます。

Windowsの検索とかファイル名を指定して実行に近い機能かな？
#### [/column]

## まとめ
Hasuraからデータがとってこれるようになったので、あとはフロントとデータベースをがつがつ進めていけるようになりました。

何なら今日のワークはフロント半分以下でした。

えるきちさんが本の予習でやっていたため、非常にスムーズに進みましたが、一からやろうとすると大変ですね。
また、関連するファイルが多いので、どこを見ているのかというのがわかりづらかった、という課題がありました。また、これまでのような画面ができた！といた目に見える進捗が出づらかった回かもしれませんね。とはいえ、いよいよ本番環境にデプロイしていくところまで進んだとも見れます。圧倒的進捗ですね。

いよいよ次はHerokuへのデプロイですね。Herokuのチームを作っておきましょう。これが、おやかたへの宿題です。

![7月30日のFun done learn](chap-mob-0730/0730fundonelearn.png?scale=0.8)

