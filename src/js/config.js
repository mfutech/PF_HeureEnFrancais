module.exports = [ {
	"type" : "heading",
	"defaultValue" : "Heure en Français"
}, {
	"type" : "text",
	"defaultValue" : "Configuration des différentes options."
}, {
	"type" : "section",
	"items" : [
	/*
	 * { "type": "heading", "defaultValue": "More Settings" },
	 */
	{
		"type" : "toggle",
		"messageKey" : "NaturalMode",
		"label" : "Arondir les minutes",
		"defaultValue" : false
	}, {
		"type" : "toggle",
		"messageKey" : "AutoReturnToTime",
		"label" : "Retour à l'heure automatique",
		"defaultValue" : true
	}, {
		"type" : "radiogroup",
		"messageKey" : "MinuteMode",
		"label" : "Mode des minutes",
		"options" : [ {
			"label" : "0-30",
			"value" : "30"
		}, {
			"label" : "0-60",
			"value" : "60"
		} ],
		"defaultValue": "30"
	}, {
		"type" : "radiogroup",
		"messageKey" : "BackgroudColor",
		"label" : "Couleur de fond",
		"options" : [ {
			"label" : "Blanc",
			"value" : "0"
		}, {
			"label" : "Noir",
			"value" : "1"
		} ],
		"defaultValue": 1
	}, {
		"type" : "toggle",
		"messageKey" : "TwentyFourMode",
		"label" : "Mode 24h",
		"defaultValue": false
	} ]
}, {
	"type" : "submit",
	"defaultValue" : "Sauvegarder"
} ];