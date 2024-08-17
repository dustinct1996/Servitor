#include <dpp/dpp.h>

int main() {
	dpp::cluster bot(getenv("BOT_TOKEN"));

	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "addfaction") {
			auto faction_id = std::get<dpp::snowflake>(event.get_parameter("faction"));

			if (faction_id == 1273860799722426410 || 
				faction_id == 1273838126883930156 || 
				faction_id == 1273859829022195713 || 
				faction_id == 1273861291038998559) {
				event.reply("+++ PERMISSION DENIED +++");
				return;
			}

			std::string faction_name = dpp::find_role(faction_id)->name;

			std::vector<dpp::snowflake> user_roles = event.command.member.get_roles();

			for (dpp::snowflake role : user_roles) {
				if (role == faction_id) {
					event.reply("You are already a part of the " + faction_name + ".");
					return;
				}
			}

			bot.guild_member_add_role(event.command.guild_id, event.command.member.user_id, faction_id);

			srand(time(0));

			switch (rand() % 3) {
			case 0:
				event.reply("You have joined the " + faction_name + ". Awaiting further instructions...");
				break;
			case 1:
				event.reply("You have been successfully added to the " + faction_name + ". Standing by...");
				break;
			case 2:
				event.reply("Your will be done. You have been added to the " + faction_name + ".");
				break;
			}

		} else if (event.command.get_command_name() == "remfaction") {
			auto val = event.get_parameter("faction");
			dpp::snowflake faction_id = std::get<dpp::snowflake>(event.get_parameter("faction"));

			if (faction_id == 1273860799722426410 ||
				faction_id == 1273838126883930156 ||
				faction_id == 1273859829022195713 ||
				faction_id == 1273861291038998559) {
				event.reply("+++ PERMISSION DENIED +++");
				return;
			}

			std::string faction_name = dpp::find_role(faction_id)->name;

			std::vector<dpp::snowflake> user_roles = event.command.member.get_roles();

			for (dpp::snowflake role : user_roles) {
				if (role == faction_id) {
					bot.guild_member_delete_role(event.command.guild_id, event.command.member.user_id, faction_id);

					srand(time(0));

					switch (rand() % 3) {
					case 0:
						event.reply("You have been successfully removed from the " + faction_name + ".");
						break;
					case 1:
						event.reply("Yes, master. You are no longer part of the " + faction_name + ".");
						break;
					case 2:
						event.reply("You have left the " + faction_name + ", master. Pending further orders...");
						break;
					}
					return;
				}
			}

			event.reply("Apologies, master, but you are not a part of the " + faction_name + ".");
		}
	});

	bot.on_ready([&bot](const dpp::ready_t& event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			dpp::slashcommand add_faction("addfaction", "Choose your faction.", bot.me.id);
			add_faction.add_option(dpp::command_option(dpp::co_role, "faction", "Faction name", true));
			bot.global_command_create(add_faction);

			dpp::slashcommand rem_faction("remfaction", "Leave a faction.", bot.me.id);
			rem_faction.add_option(dpp::command_option(dpp::co_role, "faction", "Faction name", true));
			bot.global_command_create(rem_faction);
		}
	});

	bot.start(dpp::st_wait);

	return 0;
}