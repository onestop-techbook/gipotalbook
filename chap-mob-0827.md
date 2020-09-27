# DB設計をHasuraに反映した （モブワーク#15:2020/8/27）

昨日のDB設計に引き続き、設計したDBをHasuraへ設定してDB実装を進めます。

## テーブルを作る 

作り始める準備で、いろいろ起動します。前回までに作った構成にて、Pullしたローカル環境で想定通り表示されることを確認します。

```sh
$ npm install @apollo/react-hooks　#最近バージョンアップしたのでその反映
$ npm run dev
$ npm run hasura console
```

ローカルのHasuraにデータを入れると、ローカルにMigration(DBの差分ファイルのようなもの)ができます。そしてMigrationをMasterにPushしたらマージするGitHub Actionsを設定してあるので、MigrationがリモートのHasuraに取り込まれます。

そういうわけで、先日作ったテーブル定義のスプレッドシートをみながら、ローカルのHasuraのSchemaに昨日作ったDBテーブルを追加していきます。

Table Nameをcirclesとし、id,name,description,tbf9_urlとカラムを追加していきます。型はintまたはtextを設定します。idは自動で増えてほしいので、Auto increment,Uniqueを入れておきます。Null許可なやつはチェックをつけましょう。この中ではDescriptionがそれにあたりますね。サークル説明は、ユーザーにとってはあったほうが良いのは当然ですし、省略する人はあまりいないと思いますが、なくても成立するのでNull許可です。逆にサークル名は空欄ではサービスとして成立しないのでNull不可です。このように考えて、テーブルを作っていきます。あとからでも変更はできるので、迷ったらとりあえずそれっぽい奴を選んでサクサク進めましょう。

tbf9_urlだけは、JSでのアンダースコアの扱いが少々面倒なので、Edit画面からGraphQL Field Nameをアンダースコアのないものに書き換えておきます。

ひとまず他のものもの一通り追加していきます。先日のテーブル一覧上では大文字小文字、キャメルケースやスネークケースが若干混じっていることもあるのですが、とりあえずはそのまま入力を進めます。

さて、入力項目の多いCircleItemsの入力の途中で、忘れていたものがありますが、順番の変更はできないのか？といった話が出ます。

あらかた入力が終わったところで、コミットしてPushします。好みによりますが、MigrationのSQLはひとまとめにすることができるので、Squashコマンドでまとめます。

```sh 
$ nmp run hasura migrate status
　//更新したテーブルが一覧として出る
$ nmp run hasura migrate squash --from 1598x //Error
$ npm run hasura -- migrate sqush from 1598xxx
$ nmp run hasura migrate status
　// squshedと、元になったやつがNot Presentで表示される
$ npm run hasura -- migrate apply -skip-execution --version 1598xxx //SquhedのIDを入れる
```

Unix系のコマンドでは、--の後はオプションとして解析しないで、というハックがあるようで、できました

普通ならデータベースの操作からMigrationのSchemaが生成されるのですが、サーバー側に反映されていないので、Applyコマンドでサーバー側に反映させます。

これをGitHubにコミットすると、GItHub Actionsが走って、Heroku上のHasuraに反映されます。コマンドが走ったことをかくにんしておきます。

前回は、テストだったので、取り込みまできちんとやったものではなかったので、GitHub actionsを更新しておきます。

GitHub Actions内のhasura.ymlのCLIの内容を書き換えます。

```js hasura.yml 
//変更前
    # Install CLI Test
    - name: Hasura Test
      env: 
        HASURA_GRAPHQL_ADMIN_SECRET: ${{ secrets.HASURA_GRAPHQL_ADMIN_SECRET }}
        HASURA_ENDPOINT: ${{ secrets.HASURA_ENDPOINT }}
      run: hasura migrate status --admin-secret $HASURA_GRAPHQL_ADMIN_SECRET --project hasura/ --endpoint $HASURA_ENDPOINT 
//変更後
    # Install CLI Test
    - name: Hasura Migrate
      env: 
        HASURA_GRAPHQL_ADMIN_SECRET: ${{ secrets.HASURA_GRAPHQL_ADMIN_SECRET }}
        HASURA_ENDPOINT: ${{ secrets.HASURA_ENDPOINT }}
      run: hasura migrate apply --admin-secret $HASURA_GRAPHQL_ADMIN_SECRET --project hasura/ --endpoint $HASURA_ENDPOINT 

```

Testで、ステータスを読むだけだったものを、Applyするコマンドに書き換えます。

さて、これでTableができるかと思いきや、まだできません。

原因はTableがUntrackedな状態だったためです。そこで、Hasura Console上で、Trackを連打します。

ところで、Trackとはなんでしょう？

また、MetaDataがApplyされていないことにも気づいたので、これも対応する必要があります。Schemaの方の情報はMigrateで移動できるのですが、GraphQL的な設定はMetaDataの方に入ってるのですが、MigrateだけではこのMetaDataが移行・反映されません。そこでMetaDataをApplyすることが必要です。DBのみならずMetaDataもApplyすべく、Metadata applyみたいなのを追加する必要があります。

そこで、同じように追加します。

```js hasura.yml 
    # Apply Hasura Metadata
    - name: Hasura Metadata apply
      env: 
        HASURA_GRAPHQL_ADMIN_SECRET: ${{ secrets.HASURA_GRAPHQL_ADMIN_SECRET }}
        HASURA_ENDPOINT: ${{ secrets.HASURA_ENDPOINT }}
      run: hasura metadata apply --admin-secret $HASURA_GRAPHQL_ADMIN_SECRET --project hasura/ --endpoint $HASURA_ENDPOINT 

```

これで、Migrateを動かすと、Metadataも適用されることになります。

Hasura ConsoleにてApplyされていることを確認して、おしまいです。
圧倒的進捗。


ローカルで開発したときに、別の人が開発しようとしたときに、自動で引っ張ってくることができるようにしたいところです。AさんがDBのSchemaやMetadataを更新したときに、Bさんのローカルにも引っ張ってこれるようにしたいという意図です。

# Relation・ForeignKeyを設定する

ローカルのHasuraConsoleから、RelationShipというのを張る必要があるのですが、Foreign　Keyで貼っておけば良さそうですが、一旦やってみましょう。

ModifyでForeignKeyをせっていすればよいのでしょうか？手動で張るのはめんどくさそうなので、一旦やってみましょう。

Itemsは確実にジャンルに紐付けされますので、これにForeignKeyを設定します。

Reference SchemaをPublicに、Reference Tableを　Circleitemsに、From’いd　To；GenreIDを設定しますが、ユニーク制約がついていないのでダメ、という旨のエラーが出ます。逆方向なのでダメなのかも？ということで、逆にしてみましょう。

ユニークな相手に対して、やってみるということで、CircleItemsに対して設定してみます。CircleItemsを設定し、Modifyを選び、ForeignKeyを選択し、ReferenceSchemaをPublic、Reference Tableをgenreとします。From GenreID　To; IDとします。こんどはOKでした。

同様に、Circle_Linkにも設定します。CircleLinkはCircleIDを参照しているので、ReferenceTableをCirclesを参照し、From　CircleID to IDとして、保存します。

Taggingの方にも、ReferenceTable〜tag　From tag-ID to idと設定し、もう一つ、ReferenceTable CircleItems　From item ID to IDを設定します。

Relationshipを設定します。補完されるので、確認してSaveします。特に変更は必要なさそうです。逆方向も推測してやってくれるようです。これをやらないとGraphQLとして結合されません。なお、手早くやりすぎると、データがバグることがあるので注意しましょう。

これでStatusをみてみると、ForeignKey追加が表示されています。

```sh
$ npm run hasura -- migrate sqush　--from 19xxx --name add foringkey
$ npm rum hasura --status //Sqush確認
$ npm run hasura -- migrate apply --skip-execution --version 195xxxx
```

コミットして終了、と思いきや、GitHub Actionsがコケてしまいます。
以上で、ForeignKey・Relationの設定が完了のはずが、課題は残ってしましました。

## まとめ
力尽きてしまいましたが、今日は終了です。

Hasuraのコマンドを全部覚えないといけないのは辛いですし、今ひとつ直感的で出ない動きも少なくありません。便利なのは便利ですが、ツラミはありますね。

恒例のFun done learnをやって、今日は終了ですね。

さて、この後、技術書典に向けて、各自の本の追い込みなどにより、１ヶ月ほど空いてしまいます。前の章で、9月12日の技術書典に向けてMVPを出すという目標も流れしまいます。期待してくれていたみなさま、すみません。（本章執筆：9月２８日）次の節目に向けて、実装を進めていきます（改めての決意！）｀s
