// -*- coding: utf-8 -*-
//
// Addon.hxx --- FlightGear class holding add-on metadata
// Copyright (C) 2017  Florent Rougon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef FG_ADDON_HXX
#define FG_ADDON_HXX

#include <ostream>
#include <string>

#include <simgear/misc/sg_path.hxx>
#include <simgear/nasal/cppbind/NasalHash.hxx>
#include <simgear/nasal/naref.h>
#include <simgear/props/props.hxx>
#include <simgear/structure/SGReferenced.hxx>

#include "addon_fwd.hxx"
#include "AddonVersion.hxx"

namespace flightgear
{

class Addon : public SGReferenced {
public:
  // Default constructor. 'minFGVersionRequired' is initialized to "2017.4.0"
  // and 'maxFGVersionRequired' to "none".
  Addon();
  // An empty value for 'minFGVersionRequired' is translated into "2017.4.0".
  // An empty value for 'maxFGVersionRequired' is translated into "none".
  Addon(std::string id, AddonVersion version, SGPath basePath,
        std::string minFGVersionRequired = "",
        std::string maxFGVersionRequired = "",
        SGPropertyNode* addonNode = nullptr);

  std::string getId() const;
  void setId(const std::string& addonId);

  std::string getName() const;
  void setName(const std::string& addonName);

  AddonVersionRef getVersion() const;
  void setVersion(const AddonVersion& addonVersion);

  std::string getShortDescription() const;
  void setShortDescription(const std::string& addonShortDescription);

  std::string getLongDescription() const;
  void setLongDescription(const std::string& addonLongDescription);

  SGPath getBasePath() const;
  void setBasePath(const SGPath& addonBasePath);

  // Should be valid for use with simgear::strutils::compare_versions()
  std::string getMinFGVersionRequired() const;
  void setMinFGVersionRequired(const std::string& minFGVersionRequired);

  // Should be valid for use with simgear::strutils::compare_versions(),
  // except for the special value "none".
  std::string getMaxFGVersionRequired() const;
  void setMaxFGVersionRequired(const std::string& maxFGVersionRequired);

  std::string getHomePage() const;
  void setHomePage(const std::string& addonHomePage);

  std::string getDownloadUrl() const;
  void setDownloadUrl(const std::string& addonDownloadUrl);

  std::string getSupportUrl() const;
  void setSupportUrl(const std::string& addonSupportUrl);

  // Node pertaining to the add-on in the Global Property Tree
  SGPropertyNode_ptr getAddonNode() const;
  void setAddonNode(SGPropertyNode* addonNode);
  // For Nasal: result as a props.Node object
  naRef getAddonPropsNode() const;

  // Property node indicating whether the add-on is fully loaded
  SGPropertyNode_ptr getLoadedFlagNode() const;

  // 0 for the first loaded add-on, 1 for the second, etc.
  // -1 means “not set” (as done by the default constructor)
  int getLoadSequenceNumber() const;
  void setLoadSequenceNumber(int num);

  // Simple string representation
  std::string str() const;

  static void setupGhost(nasal::Hash& addonsModule);

private:
  // The add-on identifier, in reverse DNS style. The AddonManager refuses to
  // register two add-ons with the same id in a given FlightGear session.
  std::string _id;
  // Pretty name for the add-on (not constrained to reverse DNS style)
  std::string _name;
  // Use a smart pointer to expose the AddonVersion instance to Nasal without
  // needing to copy the data every time.
  AddonVersionRef _version;
  // Strings describing what the add-on does
  std::string _shortDescription;
  std::string _longDescription;
  SGPath _basePath;
  // To be used with simgear::strutils::compare_versions()
  std::string _minFGVersionRequired;
  // Ditto, but there is a special value: "none"
  std::string _maxFGVersionRequired;
  std::string _homePage;
  std::string _downloadUrl;
  std::string _supportUrl;
  // Main node for the add-on in the Property Tree
  SGPropertyNode_ptr _addonNode;
  // Semantics explained above
  int _loadSequenceNumber = -1;
};

std::ostream& operator<<(std::ostream& os, const Addon& addonMetaData);

} // of namespace flightgear

#endif  // of FG_ADDON_HXX