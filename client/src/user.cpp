#include "user.h"

User::User()
{
}

QJsonObject SelfInfo::get_basic_info_json()
{

    QJsonObject json_obj;
    json_obj["uid"] = uid_;
    json_obj["token"] = token_;

    return json_obj;
}

User::User(QString uid, QString name, QString avatar)
{

    uid_ = uid;
    name_ = name;
    avatar_ = avatar;
}

FriendApply::FriendApply(User from_user, User to_user, QString remark_name, QString apply_message, int status)
{
    from_user_ = from_user;
    to_user_ = to_user;
    remark_name_ = remark_name;
    apply_message_ = apply_message;
    status_ = status;
}
