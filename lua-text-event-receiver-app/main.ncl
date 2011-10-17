<?xml version="1.0" encoding="ISO-8859-1"?>
<ncl id="nclClicks" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">

<head>

<regionBase>
    <region width="20%"  height="20%"  left="10%"  top="40%" id="rgBt"/>
    <region width="50%"  height="50%"  right="10%" top="25%" id="rgLua"/>
</regionBase>

<descriptorBase>
    <descriptor id="dsLua" region="rgLua" focusIndex="ixLua"/>
    <descriptor id="dsBt" region="rgBt"/>
</descriptorBase>

<connectorBase>
	<causalConnector id="onBeginStart">
		<simpleCondition role="onBegin"/>
		<simpleAction role="start" max="unbounded"/>
	</causalConnector>

	<causalConnector id="onEndStopStart">
		<simpleCondition role="onEnd"/>
		<compoundAction operator="seq">
			<simpleAction role="stop"/>
			<simpleAction role="start"/>
		</compoundAction>
	</causalConnector>
</connectorBase>

</head>

<body>
	<!-- Propriedas -->
	<media type="application/x-ginga-settings" id="programSettings">
		<!-- Essa propriedade é necessária caso se queira que o foco vá para um
			elemento diferente do que possui o menor focusIndex. -->
        <property name="service.currentKeyMaster" value="ixLua"/>
	</media>

    <port id="entryPoint" component="lua"/>

	<!-- MEDIAS -->
    <media id="lua"  src="mm.lua" descriptor="dsLua"/>

	<media id="bt_green" src="buttons/1_green.png" descriptor="dsBt"/>
	<media id="bt_red"   src="buttons/1_red.png"   descriptor="dsBt"/>
	
	<!-- BEGIN LUA: STARTS RUN_BUTTON -->
	<link xconnector="onBeginStart">
		<bind role="onBegin" component="lua"/>
		<bind role="start"   component="bt_green"/>
	</link>

	<!-- END LUA: STOPS RUN_BUTTON, STARTS END_BUTTON -->
	<link xconnector="onEndStopStart">
		<bind role="onEnd" component="lua"/>
		<bind role="stop"  component="bt_green"/>
		<bind role="start" component="bt_red"/>
	</link>

</body>

</ncl>
