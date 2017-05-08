/****************************************************************************/
/// @file    GNEChange_Connection.cpp
/// @author  Jakob Erdmann
/// @date    May 2011
/// @version $Id$
///
// A network change in which a single connection is created or deleted
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2001-2017 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <utils/common/MsgHandler.h>
#include <cassert>

#include "GNEChange_Connection.h"
#include "GNEConnection.h"
#include "GNEEdge.h"
#include "GNENet.h"
#include "GNEViewNet.h"


// ===========================================================================
// FOX-declarations
// ===========================================================================
FXIMPLEMENT_ABSTRACT(GNEChange_Connection, GNEChange, NULL, 0)

// ===========================================================================
// member method definitions
// ===========================================================================


GNEChange_Connection::GNEChange_Connection(GNEEdge* edge, NBEdge::Connection nbCon, bool selected, bool forward) :
    GNEChange(edge->getNet(), forward),
    myEdge(edge),
    myNBEdgeConnection(nbCon),
    mySelected(selected) {
    assert(myEdge);
}


GNEChange_Connection::~GNEChange_Connection() {
    assert(myEdge);
}


void
GNEChange_Connection::undo() {
    if (myForward) {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing") == true) {
            WRITE_WARNING("Removing " + toString(SUMO_TAG_CONNECTION) + " '" + 
                          myEdge->getNBEdge()->getLaneID(myNBEdgeConnection.fromLane) + "->" + myNBEdgeConnection.toEdge->getLaneID(myNBEdgeConnection.toLane) + "' from " + 
                          toString(SUMO_TAG_EDGE) + " '" + myEdge->getID() + "'");
        }
        // remove connection from edge
        myEdge->removeConnection(myNBEdgeConnection);
    } else {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing") == true) {
            std::string selected = (mySelected == true)? ("a previously selected ") : ("");
            WRITE_WARNING("Adding " + selected + toString(SUMO_TAG_CONNECTION) + " '" + 
                          myEdge->getNBEdge()->getLaneID(myNBEdgeConnection.fromLane) + "->" + myNBEdgeConnection.toEdge->getLaneID(myNBEdgeConnection.toLane) + "' into " + 
                          toString(SUMO_TAG_EDGE) + " '" + myEdge->getID() + "'");
        }
        // add connection into edge
        myEdge->addConnection(myNBEdgeConnection, mySelected);
    }
}


void
GNEChange_Connection::redo() {
    if (myForward) {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing") == true) {
            std::string selected = (mySelected == true)? ("a previously selected ") : ("");
            WRITE_WARNING("Adding " + selected + toString(SUMO_TAG_CONNECTION) + " '" + 
                          myEdge->getNBEdge()->getLaneID(myNBEdgeConnection.fromLane) + "->" + myNBEdgeConnection.toEdge->getLaneID(myNBEdgeConnection.toLane) + "' into " + 
                          toString(SUMO_TAG_EDGE) + " '" + myEdge->getID() + "'");
        }
        // add connection into edge
        myEdge->addConnection(myNBEdgeConnection, mySelected);
    } else {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing") == true) {
            WRITE_WARNING("Removing " + toString(SUMO_TAG_CONNECTION) + " '" + 
                          myEdge->getNBEdge()->getLaneID(myNBEdgeConnection.fromLane) + "->" + myNBEdgeConnection.toEdge->getLaneID(myNBEdgeConnection.toLane) + "' from " + 
                          toString(SUMO_TAG_EDGE) + " '" + myEdge->getID() + "'");
        }
        // remove connection from edge
        myEdge->removeConnection(myNBEdgeConnection);
    }
}


FXString
GNEChange_Connection::undoName() const {
    if (myForward) {
        return ("Undo create " + toString(SUMO_TAG_CONNECTION)).c_str();
    } else {
        return ("Undo delete " + toString(SUMO_TAG_CONNECTION)).c_str();
    }
}


FXString
GNEChange_Connection::redoName() const {
    if (myForward) {
        return ("Redo create " + toString(SUMO_TAG_CONNECTION)).c_str();
    } else {
        return ("Redo delete " + toString(SUMO_TAG_CONNECTION)).c_str();
    }
}
