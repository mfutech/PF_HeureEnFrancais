module.exports = [ {
	"type" : "heading",
	"defaultValue" : "Heure en Français"
}, 
/*
 * {
	"type" : "text",
	"defaultValue" : "Configuration des différentes options."
	},
*/
{
	"type" : "section",
	"items" : [
	/*
	 * { "type": "heading", "defaultValue": "More Settings" },
	 */
	{
		"type" : "toggle",
		"messageKey" : "RoundedMode",
		"label" : "Arondir les minutes",
		"defaultValue" : false
	}, {
		"type" : "toggle",
		"messageKey" : "AutoReturnToTime",
		"label" : "Retour à l'heure automatique",
		"defaultValue" : true
	},  {
		"type" : "radiogroup",
		"messageKey" : "NaturalMode",
		"label" : "Présentation des minutes",
		"options" : [ {
			"label" : "0-30",
			"value" : "0"
		}, {
			"label" : "0-59",
			"value" : "1"
		} ],
		"defaultValue": "0"
	}, {
		"type" : "radiogroup",
		"messageKey" : "RevertColor",
		"label" : "Couleur de fond",
		"options" : [ {
			"label" : "Blanc",
			"value" : "0"
		}, {
			"label" : "Noir",
			"value" : "1"
		} ],
		"defaultValue": "1"
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