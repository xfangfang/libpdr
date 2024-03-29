//
// Created by fang on 2023/7/20.
//

#include "libpdr.h"

// clang-format off

namespace pdr {

    const std::string RendererServiceAVTransport::AVTransport = R"xml(<?xml version="1.0" encoding="UTF-8"?>
<scpd xmlns="urn:schemas-upnp-org:service-1-0">
    <specVersion>
        <major>1</major>
        <minor>0</minor>
    </specVersion>
    <actionList>
        <action>
            <name>GetCurrentTransportActions</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Actions</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentTransportActions</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetDeviceCapabilities</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>PlayMedia</name>
                    <direction>out</direction>
                    <relatedStateVariable>PossiblePlaybackStorageMedia</relatedStateVariable>
                </argument>
                <argument>
                    <name>RecMedia</name>
                    <direction>out</direction>
                    <relatedStateVariable>PossibleRecordStorageMedia</relatedStateVariable>
                </argument>
                <argument>
                    <name>RecQualityModes</name>
                    <direction>out</direction>
                    <relatedStateVariable>PossibleRecordQualityModes</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetMediaInfo</name>
            <argumentList>
                <argument>)xml" R"xml(
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>NrTracks</name>
                    <direction>out</direction>
                    <relatedStateVariable>NumberOfTracks</relatedStateVariable>
                </argument>
                <argument>
                    <name>MediaDuration</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentMediaDuration</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentURI</name>
                    <direction>out</direction>
                    <relatedStateVariable>AVTransportURI</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentURIMetaData</name>
                    <direction>out</direction>
                    <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>
                </argument>
                <argument>
                    <name>NextURI</name>
                    <direction>out</direction>
                    <relatedStateVariable>NextAVTransportURI</relatedStateVariable>
                </argument>
                <argument>
                    <name>NextURIMetaData</name>
                    <direction>out</direction>
                    <relatedStateVariable>NextAVTransportURIMetaData</relatedStateVariable>
                </argument>
                <argument>
                    <name>PlayMedium</name>
                    <direction>out</direction>
                    <relatedStateVariable>PlaybackStorageMedium</relatedStateVariable>
                </argument>
                <argument>
                    <name>RecordMedium</name>
                    <direction>out</direction>)xml" R"xml(
                    <relatedStateVariable>RecordStorageMedium</relatedStateVariable>
                </argument>
                <argument>
                    <name>WriteStatus</name>
                    <direction>out</direction>
                    <relatedStateVariable>RecordMediumWriteStatus</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetPositionInfo</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Track</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentTrack</relatedStateVariable>
                </argument>
                <argument>
                    <name>TrackDuration</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentTrackDuration</relatedStateVariable>
                </argument>
                <argument>
                    <name>TrackMetaData</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentTrackMetaData</relatedStateVariable>
                </argument>
                <argument>
                    <name>TrackURI</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentTrackURI</relatedStateVariable>
                </argument>
                <argument>
                    <name>RelTime</name>
                    <direction>out</direction>
                    <relatedStateVariable>RelativeTimePosition</relatedStateVariable>
                </argument>)xml" R"xml(
                <argument>
                    <name>AbsTime</name>
                    <direction>out</direction>
                    <relatedStateVariable>AbsoluteTimePosition</relatedStateVariable>
                </argument>
                <argument>
                    <name>RelCount</name>
                    <direction>out</direction>
                    <relatedStateVariable>RelativeCounterPosition</relatedStateVariable>
                </argument>
                <argument>
                    <name>AbsCount</name>
                    <direction>out</direction>
                    <relatedStateVariable>AbsoluteCounterPosition</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetTransportInfo</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentTransportState</name>
                    <direction>out</direction>
                    <relatedStateVariable>TransportState</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentTransportStatus</name>
                    <direction>out</direction>
                    <relatedStateVariable>TransportStatus</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentSpeed</name>
                    <direction>out</direction>
                    <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>
                </argument>
            </argumentList>
        </action>)xml" R"xml(
        <action>
            <name>GetTransportSettings</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>PlayMode</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentPlayMode</relatedStateVariable>
                </argument>
                <argument>
                    <name>RecQualityMode</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentRecordQualityMode</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>Next</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>Pause</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
            </argumentList>
        </action>)xml" R"xml(
        <action>
            <name>Play</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Speed</name>
                    <direction>in</direction>
                    <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>Previous</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>Seek</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Unit</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_SeekMode</relatedStateVariable>
                </argument>
                <argument>
                    <name>Target</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_SeekTarget</relatedStateVariable>
                </argument>
            </argumentList>
        </action>)xml" R"xml(
        <action>
            <name>SetAVTransportURI</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentURI</name>
                    <direction>in</direction>
                    <relatedStateVariable>AVTransportURI</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentURIMetaData</name>
                    <direction>in</direction>
                    <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>SetPlayMode</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>NewPlayMode</name>
                    <direction>in</direction>
                    <relatedStateVariable>CurrentPlayMode</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>Stop</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
    </actionList>)xml" R"xml(
    <serviceStateTable>
        <stateVariable sendEvents="no">
            <name>CurrentPlayMode</name>
            <dataType>string</dataType>
            <defaultValue>NORMAL</defaultValue>
            <allowedValueList>
                <allowedValue>NORMAL</allowedValue>
                <allowedValue>REPEAT_ONE</allowedValue>
                <allowedValue>REPEAT_ALL</allowedValue>
                <allowedValue>SHUFFLE</allowedValue>
                <allowedValue>SHUFFLE_NOREPEAT</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>RecordStorageMedium</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NOT_IMPLEMENTED</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="yes">
            <name>LastChange</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>RelativeTimePosition</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentTrackTitle</name>
            <dataType>string</dataType>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>DisplayCurrentSubtitle</name>
            <dataType>boolean</dataType>
            <defaultValue>1</defaultValue>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentTrackURI</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentTrackDuration</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentRecordQualityMode</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NOT_IMPLEMENTED</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentMediaDuration</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>AbsoluteCounterPosition</name>
            <dataType>i4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>RelativeCounterPosition</name>
            <dataType>i4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_InstanceID</name>
            <dataType>ui4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>AVTransportURI</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>TransportState</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>STOPPED</allowedValue>
                <allowedValue>PAUSED_PLAYBACK</allowedValue>
                <allowedValue>PLAYING</allowedValue>
                <allowedValue>TRANSITIONING</allowedValue>
                <allowedValue>NO_MEDIA_PRESENT</allowedValue>
            </allowedValueList>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>CurrentTrackMetaData</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>NextAVTransportURI</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>PossibleRecordQualityModes</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NOT_IMPLEMENTED</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentTrack</name>
            <dataType>ui4</dataType>
            <allowedValueRange>
                <minimum>0</minimum>
                <maximum>65535</maximum>
                <step>1</step>
            </allowedValueRange>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>AbsoluteTimePosition</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>NextAVTransportURIMetaData</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>PlaybackStorageMedium</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NONE</allowedValue>
                <allowedValue>UNKNOWN</allowedValue>
                <allowedValue>CD-DA</allowedValue>
                <allowedValue>HDD</allowedValue>
                <allowedValue>NETWORK</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>CurrentTransportActions</name>
            <dataType>string</dataType>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>RecordMediumWriteStatus</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NOT_IMPLEMENTED</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>PossiblePlaybackStorageMedia</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NONE</allowedValue>
                <allowedValue>UNKNOWN</allowedValue>
                <allowedValue>CD-DA</allowedValue>
                <allowedValue>HDD</allowedValue>
                <allowedValue>NETWORK</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>AVTransportURIMetaData</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>NumberOfTracks</name>
            <dataType>ui4</dataType>
            <allowedValueRange>
                <minimum>0</minimum>
                <maximum>65535</maximum>
            </allowedValueRange>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_SeekMode</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>REL_TIME</allowedValue>
                <allowedValue>TRACK_NR</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_SeekTarget</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>PossibleRecordStorageMedia</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>NOT_IMPLEMENTED</allowedValue>
            </allowedValueList>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>TransportStatus</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>OK</allowedValue>
                <allowedValue>ERROR_OCCURRED</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>TransportPlaySpeed</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>0.5</allowedValue>
                <allowedValue>0.75</allowedValue>
                <allowedValue>1</allowedValue>
                <allowedValue>1.25</allowedValue>
                <allowedValue>1.5</allowedValue>
                <allowedValue>2</allowedValue>
            </allowedValueList>
        </stateVariable>
    </serviceStateTable>
</scpd>
)xml";

const std::string RendererServiceRenderingControl::RenderingControl = R"xml(<?xml version="1.0" encoding="UTF-8"?>
<scpd xmlns="urn:schemas-upnp-org:service-1-0">
    <specVersion>
        <major>1</major>
        <minor>0</minor>
    </specVersion>
    <actionList>
        <action>
            <name>GetMute</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentMute</name>
                    <direction>out</direction>
                    <relatedStateVariable>Mute</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetVolume</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentVolume</name>
                    <direction>out</direction>
                    <relatedStateVariable>Volume</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetVolumeDB</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>)xml" R"xml(
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>CurrentVolume</name>
                    <direction>out</direction>
                    <relatedStateVariable>VolumeDB</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetVolumeDBRange</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>MinValue</name>
                    <direction>out</direction>
                    <relatedStateVariable>VolumeDB</relatedStateVariable>
                </argument>
                <argument>
                    <name>MaxValue</name>
                    <direction>out</direction>
                    <relatedStateVariable>VolumeDB</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>ListPresets</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>)xml" R"xml(
                <argument>
                    <name>CurrentPresetNameList</name>
                    <direction>out</direction>
                    <relatedStateVariable>PresetNameList</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>SelectPreset</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>PresetName</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_PresetName</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>SetMute</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>DesiredMute</name>
                    <direction>in</direction>
                    <relatedStateVariable>Mute</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>SetVolume</name>
            <argumentList>
                <argument>
                    <name>InstanceID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>
                </argument>)xml" R"xml(
                <argument>
                    <name>Channel</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>
                </argument>
                <argument>
                    <name>DesiredVolume</name>
                    <direction>in</direction>
                    <relatedStateVariable>Volume</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
    </actionList>
    <serviceStateTable>
        <stateVariable sendEvents="yes">
            <name>LastChange</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_Channel</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>Master</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_InstanceID</name>
            <dataType>ui4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>Volume</name>
            <dataType>ui2</dataType>
            <allowedValueRange>
                <minimum>0</minimum>
                <maximum>100</maximum>
                <step>1</step>
            </allowedValueRange>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>Mute</name>
            <dataType>boolean</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>PresetNameList</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>FactoryDefaults</allowedValue>
            </allowedValueList>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_PresetName</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>FactoryDefaults</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>VolumeDB</name>
            <dataType>i2</dataType>
            <allowedValueRange>
                <minimum>-32767</minimum>
                <maximum>32767</maximum>
            </allowedValueRange>
        </stateVariable>
    </serviceStateTable>
</scpd>
)xml";

const std::string RendererServiceConnectionManager::ConnectionManager = R"xml(<?xml version="1.0" encoding="UTF-8"?>
<scpd xmlns="urn:schemas-upnp-org:service-1-0">
    <specVersion>
        <major>1</major>
        <minor>0</minor>
    </specVersion>
    <actionList>
        <action>
            <name>GetCurrentConnectionInfo</name>
            <argumentList>
                <argument>
                    <name>ConnectionID</name>
                    <direction>in</direction>
                    <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>
                </argument>
                <argument>
                    <name>RcsID</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_RcsID</relatedStateVariable>
                </argument>
                <argument>
                    <name>AVTransportID</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_AVTransportID</relatedStateVariable>
                </argument>
                <argument>
                    <name>ProtocolInfo</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_ProtocolInfo</relatedStateVariable>
                </argument>
                <argument>
                    <name>PeerConnectionManager</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_ConnectionManager</relatedStateVariable>
                </argument>
                <argument>
                    <name>PeerConnectionID</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>
                </argument>
                <argument>
                    <name>Direction</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_Direction</relatedStateVariable>
                </argument>)xml" R"xml(
                <argument>
                    <name>Status</name>
                    <direction>out</direction>
                    <relatedStateVariable>A_ARG_TYPE_ConnectionStatus</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetProtocolInfo</name>
            <argumentList>
                <argument>
                    <name>Source</name>
                    <direction>out</direction>
                    <relatedStateVariable>SourceProtocolInfo</relatedStateVariable>
                </argument>
                <argument>
                    <name>Sink</name>
                    <direction>out</direction>
                    <relatedStateVariable>SinkProtocolInfo</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
        <action>
            <name>GetCurrentConnectionIDs</name>
            <argumentList>
                <argument>
                    <name>ConnectionIDs</name>
                    <direction>out</direction>
                    <relatedStateVariable>CurrentConnectionIDs</relatedStateVariable>
                </argument>
            </argumentList>
        </action>
    </actionList>
    <serviceStateTable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_ProtocolInfo</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_ConnectionStatus</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>OK</allowedValue>
                <allowedValue>ContentFormatMismatch</allowedValue>
                <allowedValue>InsufficientBandwidth</allowedValue>
                <allowedValue>UnreliableChannel</allowedValue>
                <allowedValue>Unknown</allowedValue>
            </allowedValueList>
        </stateVariable>)xml" R"xml(
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_AVTransportID</name>
            <dataType>i4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_RcsID</name>
            <dataType>i4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_ConnectionID</name>
            <dataType>i4</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_ConnectionManager</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="yes">
            <name>SourceProtocolInfo</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="yes">
            <name>SinkProtocolInfo</name>
            <dataType>string</dataType>
        </stateVariable>
        <stateVariable sendEvents="no">
            <name>A_ARG_TYPE_Direction</name>
            <dataType>string</dataType>
            <allowedValueList>
                <allowedValue>Input</allowedValue>
                <allowedValue>Output</allowedValue>
            </allowedValueList>
        </stateVariable>
        <stateVariable sendEvents="yes">
            <name>CurrentConnectionIDs</name>
            <dataType>string</dataType>
        </stateVariable>
    </serviceStateTable>
</scpd>
)xml";
}