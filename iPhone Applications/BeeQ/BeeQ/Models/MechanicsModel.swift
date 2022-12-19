//
//  MechanicsModel.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import Foundation

struct GameMechanics {
    let actionTypes: [String]
    var buttons: [ButtonModel]
    init () {
        actionTypes = ["delete.left.fill", "restart.circle", "square.and.arrow.up.circle"]
        buttons = [ButtonModel(uid: 0, icon: actionTypes[0], isDisabled: true), ButtonModel(uid: 1, icon: actionTypes[1], isDisabled: false), ButtonModel(uid: 2, icon: actionTypes[2], isDisabled: true)]
    }
}

struct ButtonModel: Hashable {
    let id: Int
    var ico: String
    var disabled: Bool
    init (uid: Int, icon: String, isDisabled: Bool) {
        id = uid
        ico = icon
        disabled = isDisabled
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(id)
    }
}
