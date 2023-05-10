//
//  PentominoesApp.swift
//  Pentominoes
//
//  Created by user224354 on 9/14/22.
//

import SwiftUI

@main
struct PentominoesApp: App {
    @StateObject var manager: PentominoManager = PentominoManager()
    var body: some Scene {
        WindowGroup {
            MainView()
                .environmentObject(manager)
        }
    }
}
