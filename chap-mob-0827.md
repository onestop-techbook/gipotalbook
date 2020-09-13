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

そういうわけで、ローカルのHasuraのSchemaに昨日作ったDBテーブルを追加していきます。

Table Nameをcirclesとし、id,name,description,tbf9_urlとカラムを追加していきます。型はintまたはtextを設定します。idは自動で増えてほしいので、Auto increment,Uniqueを入れておきます。Null許可なやつはチェックをつけましょう。この中ではDescriptionがそれにあたりますね。あとからでも変更はできるので、迷ったらとりあえずそれっぽい奴を選んでサクサク進めましょう。

tbf9_urlだけは、JSでのアンダースコアの扱いが少々面倒なので、Edit画面からGraphQL Field Nameをアンダースコアのないものに書き換えておきます。

ひとまず他のものもの一通り追加していきます。大文字小文字、キャメルケースやスネークケースが若干混じっていることもあるのですが、とりあえずはそのまま入力を進めます。

入力項目の多いCircleItemsの入力の途中で、忘れていたものがありますが、順番の変更はできないのか？といった話が出ます。
